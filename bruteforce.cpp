#include "common.h"
#include <vector>

void evolve_bruteforce_serial(const std::vector<particleptr>& pars) {
    int len = pars.size();
    for (int i = 0; i < len; i++) {
        double fx = 0.0f;
        double fy = 0.0f;

        for (int j = 0; j < len; j++) {
            if (j == i) continue;
            double dx = (*pars[j]).x - (*pars[i]).x;
            double dy = (*pars[j]).y - (*pars[i]).y;
            double sqdist = dx * dx + dy * dy;// +SOFTENING;
            double InvDist = 1.0 / (sqrt(sqdist));
            double InvDist3 = InvDist * InvDist * InvDist;
            fx += (dx * InvDist3 * (*pars[j]).mass) / (*pars[i]).mass;
            fy += (dy * InvDist3 * (*pars[j]).mass) / (*pars[i]).mass;
        }

        (*pars[i]).vx += timestep * fx;
        (*pars[i]).vy += timestep * fy;
        (*pars[i]).drift();
    }
}
