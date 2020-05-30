#pragma once

const double newton_g = 0.0044995611;
const double timestep = 5e-1; // simply means dt
const double box_size = 50;
const double softening = 0;
const double tree_thres = 1;
const double display_f = 200;


struct position {
	double x, y;
};


struct particle {
	position current;
	position velocity;
	double mass;

	void drift() {
		this->current.x += this->velocity.x * timestep;
		this->current.y += this->velocity.y * timestep;
		//std::cout << "vx" << velocity.x << std::endl;
		//std::cout << "vy" << velocity.y << std::endl;
	}
};













