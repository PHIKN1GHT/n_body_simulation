#pragma once
#include <vector>

const double newton_g = 0.0044995611;
const double timestep = 5e-1; // simply means dt
const double box_size = 50;
const double softening = 0;
const double tree_thres = 1;
const double display_f = 200;

struct particle {
	double x, y, vx, vy, mass;

	void drift() {
		this->x += this->vx * timestep;
		this->y += this->vy * timestep;
		//std::cout << "vx" << velocity.x << std::endl;
		//std::cout << "vy" << velocity.y << std::endl;
	}
};

typedef particle* particleptr;


extern void evolve_bruteforce_serial(const std::vector<particleptr>& pars);
extern void evolve_quadtree_serial(const std::vector<particleptr>& pars);



extern std::vector<particleptr> kepler();
extern std::vector<particleptr> dual_kepler();



