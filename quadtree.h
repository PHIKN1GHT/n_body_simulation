#pragma once
#include "common.h"
#include<vector>
#include<queue>
#include<iostream>
#include<string>
#include"simulation.h"


struct quadnode;
typedef quadnode* nodeptr;




struct quadnode {
	nodeptr child[4];
	double centerx, centery, xmin, xmax, ymin, ymax, totalMass = 0;
	particleptr within = NULL;
	int npar = 0;
	std::string name;
	//std::vector<particle> particles;
	//int particle_number;

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
		//std::cout << "adding par " << std::endl;
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
			//std::cout << "node " << this->name << " added. " << std::endl;
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
			//std::cout << "test " << dist << std::endl;
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
		double r2 = rx * rx + ry * ry;
		par.vx += newton_g * timestep * this->npar * this->totalMass / par.mass / r2 / sqrt(r2) * rx;
		par.vy += newton_g * timestep * this->npar * this->totalMass / par.mass / r2 / sqrt(r2) * ry;
		//cout << node.center.x << this->current.x << endl;
//cout << "rx" << rx << endl;
//cout << "ry" << ry << endl;



//double r = sqrt(r2);
//double x = r / softening;
//double f = x * (8 - 9 * x + 2 * x * x * x);
//this->velocity.x += newton_g * timestep * f * rx * node.npar / (softening * softening);
//this->velocity.y += newton_g * timestep * f * ry * node.npar / (softening * softening);


//cout << "r2 " <<r2 << endl;
//cout << newton_g * timestep * rx * node.npar / (r2) << endl;
//if (r2 > softening * softening) {
	//this->velocity.x += newton_g * timestep * rx * node.npar / r2 / sqrt(r2);
	//this->velocity.y += newton_g * timestep * ry * node.npar / r2 / sqrt(r2);
//}
//else {
//	double r = sqrt(r2), x = r / softening, f = x * (8 - 9 * x + 2 * x * x * x);
//	this->velocity.x += newton_g * timestep * f * rx * node.npar / (softening * softening);
//	this->velocity.y += newton_g * timestep * f * ry * node.npar / (softening * softening);
//}
	}
};


extern void evolve_quadtree_serial(const std::vector<particleptr>& pars);
extern void test_quad_tree();



/*


class Octree_serial:
  def __init__(self, particles, newton_g, box_size, timestep, tree_thres, \
	softening):

	self.particles = particles

	self.newton_g = newton_g
	self.box_size = box_size
	self.timestep = timestep
	self.tree_thres = tree_thres
	self.softening = softening

	self.timings = {"tree": 0, "force": 0}

  def evolve(self):
	# CONSTRUCT TREE
	tree_start_time = time.time()

	root = Cell(-self.box_size, self.box_size, -self.box_size, self.box_size, \
	  -self.box_size, self.box_size, "0")
	for particle in self.particles:
	  root.add(particle)

	tree_end_time = time.time()
	self.timings["tree"] += tree_end_time - tree_start_time

	# CALCULATE FORCE
	force_start_time =  time.time()
	# get particles from tree
	self.particles = root.particles()

	# for each particle,
	for particle in self.particles:
	  # traverse the tree, starting from the root node
	  # start a queue with just root node
	  cells = [root]
	  while cells:
		# take the next cell
		cell = cells.pop()
		# if this cell is far enough, use it
		if cell.meetscriterion(particle, self.tree_thres, self.softening):
		  if cell.n > 0:
			particle.kick(cell, self.newton_g, self.timestep, self.softening)
		# otherwise, try its daughters
		else:
		  cells.extend(cell.daughters)
	  # drift the particle to the next time step
	  particle.drift(self.timestep)

	force_end_time =  time.time()
	self.timings["force"] += force_end_time - force_start_time


*/





