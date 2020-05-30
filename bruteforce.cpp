#include "common.h"
#include <vector>
#include <mpi.h>

void calc_force(const int & i,  const double & len, const std::vector<particleptr>& pars, double& fx, double& fy) {
    for (int j = 0; j < len; j++) {
        if (j == i) continue;
        double dx = (*pars[j]).x - (*pars[i]).x;
        double dy = (*pars[j]).y - (*pars[i]).y;
        double sqdist = dx * dx + dy * dy < 1.0 ? 1.0 : dx * dx + dy * dy;// +SOFTENING;
        double InvDist = 1.0 / (sqrt(sqdist));
        double InvDist3 = InvDist * InvDist * InvDist;
        fx += (dx * InvDist3 * (*pars[j]).mass) / (*pars[i]).mass;
        fy += (dy * InvDist3 * (*pars[j]).mass) / (*pars[i]).mass;
    }
}

void evolve_bruteforce_serial(const std::vector<particleptr>& pars) {
    int len = pars.size();
    for (int i = 0; i < len; i++) {
        double fx = 0.0, fy = 0.0;
        calc_force(i, len, pars, fx, fy);
        (*pars[i]).vx += timestep * fx;
        (*pars[i]).vy += timestep * fy;
    }
    for (int i = 0; i < len; i++) (*pars[i]).drift();
}


void evolve_bruteforce_parallel(const std::vector<particleptr>& pars) {
    int len = pars.size(), local_len = len / MPI_SIZE + (len % MPI_SIZE != 0), offset = local_len * MPI_RANK;
    double *fx = new double[local_len], *fy = new double[local_len];

    for (int i = 0; i < local_len && offset + i < len; i++) {
        fx[i] = fy[i] = 0.0;
        calc_force(offset + i, len, pars, fx[i], fy[i]);
    }
    int newlen = local_len * MPI_SIZE;
    double* nfx = new double[newlen], *nfy = new double[newlen];

    MPI_Gather(fx, local_len, MPI_DOUBLE, nfx, local_len, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(fy, local_len, MPI_DOUBLE, nfy, local_len, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(nfx, len, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(nfy, len, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < len; i++) {
        (*pars[i]).vx += timestep * nfx[i];
        (*pars[i]).vy += timestep * nfy[i];
        (*pars[i]).drift();
    }

    delete[] fx, fy, nfx, nfy;
}