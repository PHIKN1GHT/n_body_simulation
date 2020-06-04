#pragma once
#include <vector>

const double newton_g = 0.0044995611;
const double timestep = 5e-1; // simply means dt
const double box_size = 1e6;
const double softening = 0;
const double tree_thres = 1;
const double display_f = 200;
const int DEPTH_LIM = 4;

const int NUM_THREAD = 8;
const int NUM_PAR = 40000;

struct particle {
	double x, y, vx, vy, mass;

	void drift() {
		this->x += this->vx * timestep;
		this->y += this->vy * timestep;
	}
};

typedef particle* particleptr;


extern void evolve_bruteforce_serial(const std::vector<particleptr>& pars);
extern void evolve_bruteforce_parallel(const std::vector<particleptr>& pars);
extern void evolve_quadtree_serial(const std::vector<particleptr>& pars);
extern void evolve_quadtree_parallel(const std::vector<particleptr>& pars);

extern double softensqdist(double dist);

extern std::vector<particleptr> kepler();
extern std::vector<particleptr> dual_kepler();
extern std::vector<particleptr> random_sample(int);

extern int MPI_RANK, MPI_SIZE;
#define MPI_MASTER_COND if (MPI_RANK == 0)
#define MPI_SLAVE_COND if (MPI_RANK != 0)
