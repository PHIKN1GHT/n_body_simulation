#pragma once
#include<vector>
#include<queue>
#include<iostream>
#include<string>
#include"simulation.h"
struct position {
	double x, y;
};

struct quadnode;
typedef quadnode* nodeptr;

class particle {
public:
	position current;
	position velocity;
	double mass;

	void tttick(const quadnode& node);
	void drift() {
		
		
		this->current.x += this->velocity.x * timestep;
		this->current.y += this->velocity.y * timestep;
		//std::cout << "vx" << velocity.x << std::endl;
		//std::cout << "vy" << velocity.y << std::endl;
	}
};


typedef particle* particleptr;

struct quadnode {
	nodeptr child[4];
	position center;
	double xmin, xmax, ymin, ymax;
	particleptr within = NULL;
	int npar = 0;
	std::string name;
	double totalMass = 0;
	//std::vector<particle> particles;
	//int particle_number;

	quadnode(double xmin, double xmax, double ymin, double ymax, std::string name) {
		this->xmin = xmin;
		this->xmax = xmax;
		this->ymin = ymin;
		this->ymax = ymax;
		this->name = name;
		this->center.x = this->center.y = 0;

		this->child[0] = this->child[1] = this->child[2] = this->child[3] = NULL;

	}

	bool incell(const position& pos) & {
		return (pos.x > this->xmin && pos.x <= this->xmax) &&
			(pos.y > this->ymin && pos.y <= this->ymax);
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
		if (!this->incell(par.current)) return;
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

		this->center.x = (this->npar * this->center.x + par.current.x) / double(this->npar + 1);
		this->center.y = (this->npar * this->center.y + par.current.y) / double(this->npar + 1);
		this->npar++;
		this->totalMass += par.mass;
	}

	bool test(const particle& par) {
		if (this->child[0] != NULL) {
			double s = this->xmax - this->xmin;
			double dx = par.current.x - this->center.x;
			double dy = par.current.y - this->center.y;
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
};



struct quadtree {
	//quadnode root;

	//quadtree():root(quadnode(-box_size, box_size, -box_size, box_size, "rt")) {};

	//void addall() {
	//	quadnode p = root;

	//}

	void evolve(const std::vector<particleptr> & pars) {
		quadnode root = quadnode(-box_size, box_size, -box_size, box_size, "rt");
		
		for (auto iter = pars.begin(); iter != pars.end(); iter++)
			root.add(*(*iter));

		std::vector<particleptr> allpars = root.all();

		//std::cout << "printing all pars: " << std::endl;
		
		//for (auto iter = allpars.begin(); iter != allpars.end(); iter++) {
		//	particle p = *(*iter);
		//	std::cout << p.current.x << " " << p.current.y << " " << p.velocity.x << " " << p.velocity.y << std::endl;
		//}
		//std::cout << "printing ends." << std::endl;

		for (auto iter = allpars.begin(); iter != allpars.end(); iter++) {
			std::queue<nodeptr> nodes({&root });
			while (nodes.size()) {
				nodeptr cur = nodes.front(); nodes.pop();
				if ((*cur).test(*(*iter))) {
					if ((*cur).npar > 0)
						(*(*iter)).tttick((*cur));
				}
				else {
					if ((*cur).child[0] != NULL) nodes.push((*cur).child[0]);
					if ((*cur).child[1] != NULL) nodes.push((*cur).child[1]);
					if ((*cur).child[2] != NULL) nodes.push((*cur).child[2]);
					if ((*cur).child[3] != NULL) nodes.push((*cur).child[3]);
				}
			}


			(*(*iter)).drift();
			//std::cout << "print tree" << std::endl;
			//root.printtree();
		}
	}
};

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





