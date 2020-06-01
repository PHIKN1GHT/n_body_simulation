#pragma once
#include "common.h"
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <omp.h>

struct quadnode;
typedef quadnode* nodeptr;

struct quadnode {
	nodeptr child[4];
	double centerx, centery, xmin, xmax, ymin, ymax, totalMass = 0;
	particleptr within = NULL;
	int npar = 0;
	std::string name;

	quadnode(double xmin, double xmax, double ymin, double ymax, std::string name) {
		this->xmin = xmin;
		this->xmax = xmax;
		this->ymin = ymin;
		this->ymax = ymax;
		this->name = name;
		this->centerx = this->centery = 0;
		this->child[0] = this->child[1] = this->child[2] = this->child[3] = NULL;
	}

	bool incell(double x, double y) & {
		return (x > this->xmin && x <= this->xmax) &&
			(y > this->ymin && y <= this->ymax);
	}

	void make() {
		double xhalf = (this->xmin + this->xmax) / 2.,
			yhalf = (this->ymin + this->ymax) / 2.;
		this->child[0] = new quadnode(this->xmin, xhalf, this->ymin, yhalf, this->name + ".0");
		this->child[1] = new quadnode(this->xmin, xhalf, yhalf, this->ymax, this->name + ".1");
		this->child[2] = new quadnode(xhalf, this->xmax, this->ymin, yhalf, this->name + ".2");
		this->child[3] = new quadnode(xhalf, this->xmax, yhalf, this->ymax, this->name + ".3");
	}

	void add(const particle& par) {
		if (!this->incell(par.x, par.y)) return;
		if (this->npar > 0) {
			if (this->npar == 1) {
				this->make();
				for (int i = 0; i < 4; i++)
					(this->child[i])->add(*(this->within));
				this->within = NULL;
				
			} 
			for (int i = 0; i < 4; i++)
				(this->child[i])->add(par);
			
		} else {
			this->within = (particleptr)&par;
		}

		this->centerx = (this->npar * this->centerx + par.x) / double(this->npar + 1);
		this->centery = (this->npar * this->centery + par.y) / double(this->npar + 1);
		this->npar++;
		this->totalMass += par.mass;
	}

	bool test(const particle& par) {
		if (this->child[0] != NULL) {
			double s = this->xmax - this->xmin;
			double dx = par.x - this->centerx;
			double dy = par.y - this->centery;
			double dist = sqrt(dx * dx + dy * dy);
			return dist / s > tree_thres && dist > softening;
		} else {
			return this->within != &par;
		}
	}

	std::vector<particleptr> all() {
		if (this->within != NULL) {
			std::vector<particleptr> result;
			result.push_back(this->within);
			return result;
		}
		else {
			std::vector<particleptr> result;
			for (int i = 0; i < 4; i++) if (this->child[i] != NULL) {
				std::vector<particleptr> subresult = (this->child[i])->all();
				result.insert(result.end(), subresult.begin(), subresult.end());
			}
			return result;
		}
		
	}

	void printtree() {
		std::cout << this->name << std::endl;
		for (int i = 0; i < 4; i++)
			if (this->child[i] != NULL) this->child[i]->printtree();
	}

	void update(particle& par)& {
		double rx = this->centerx - par.x;
		double ry = this->centery - par.y;
		double r2 = softensqdist(rx * rx + ry * ry);
		par.vx += newton_g * timestep * this->npar * this->totalMass / par.mass / r2 / sqrt(r2) * rx;
		par.vy += newton_g * timestep * this->npar * this->totalMass / par.mass / r2 / sqrt(r2) * ry;
	}
};


