
#include "quadtree.h"
#include <iostream>
using namespace std;


void particle::tttick(const quadnode& node) {
	double rx = node.center.x - this->current.x;
	double ry = node.center.y - this->current.y;

	//cout << node.center.x << this->current.x << endl;
	//cout << "rx" << rx << endl;
	//cout << "ry" << ry << endl;


	double r2 = rx * rx + ry * ry;
	this->velocity.x += newton_g * timestep * node.npar * node.totalMass / this->mass / r2 / sqrt(r2) * rx;
	this->velocity.y += newton_g * timestep * node.npar * node.totalMass / this->mass / r2 / sqrt(r2) * ry;
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
};





void test_quad_tree(){
	//cout << "hello, world" << endl;

	std::vector<particleptr> particles;
	double a = 10, e = 0.; double rmin = a * (1 - e), h = sqrt((rmin * (1 + e) * newton_g * 2)),
		v = h / rmin;
	particle p1 = particle({ {(double)(rmin / 2.), (double)0}, {(double)0,  (v / 2) } , 3});
	particle p2 = particle({ {(double)(-rmin / 2.), (double)0}, {(double)0, (-v / 2)} , 1});
	particles.push_back(&p1); particles.push_back(&p2);
	quadtree tree;
	//tree.addall(particles);
	for (int i = 0; i < 1000; i++) {
		cout << "P1: " << p1.current.x << " " << p1.current.y << " " << p1.velocity.x << " " << p1.velocity.y << "  ";
		cout << "P2: " << p2.current.x << " " << p2.current.y << " " << p2.velocity.x << " " << p2.velocity.y << endl;
		
		tree.evolve(particles);
	}


	
	

	//particles = InitialConditions.kepler(10, 0., newton_g)
	//def kepler(a, e, NEWTON_G):
	// #periastron
	//	 #angular momentum
	//	
	//	return[Particle(rmin / 2., 0, 0, 0, 0, v / 2., "A"), \
	//	Particle(-rmin / 2., 0, 0, 0, 0, -v / 2., "B")]

}










