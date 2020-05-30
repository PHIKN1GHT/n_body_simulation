
#include "quadtree.h"
#include <iostream>
using namespace std;



void evolve_quadtree_serial(const std::vector<particleptr>& pars) {
	quadnode root = quadnode(-box_size, box_size, -box_size, box_size, "rt");
	for (auto iter = pars.begin(); iter != pars.end(); iter++)
		root.add(*(*iter));

	std::vector<particleptr> allpars = root.all();
	for (auto iter = allpars.begin(); iter != allpars.end(); iter++) {
		std::queue<nodeptr> nodes({ &root });
		while (nodes.size()) {
			nodeptr cur = nodes.front(); nodes.pop();
			if ((*cur).test(*(*iter))) {
				if ((*cur).npar > 0)
					(*cur).update(*(*iter));
				//(*(*iter)).tttick((*cur));
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


void test_quad_tree(){
	//cout << "hello, world" << endl;

	
	
	//particle p1 = particle({ {(double)(rmin / 2.), (double)0}, {(double)0,  (v / 2) } , 1});
	//particle p2 = particle({ {(double)(-rmin / 2.), (double)0}, {(double)0, (-v / 2)} , 1});
	//particle p3 = particle({ {(double)0, (double)(rmin / 2.)}, {(v / 2), (double)0} , 1 });
	//particle p4 = particle({ {(double)0, (double)(-rmin / 2.)}, {(-v / 2), (double)0} , 1 });
	//particles.push_back(&p1); particles.push_back(&p2);


	auto particles = dual_kepler();

	//tree.addall(particles);
	for (int i = 0; i < 3; i++) {
		for (auto par : particles)
			cout << (*par).x << " " << (*par).y << " ";
		cout << endl;

		//evolve_quadtree_serial(particles);
		evolve_bruteforce_serial(particles);
	}


	
	

	//particles = InitialConditions.kepler(10, 0., newton_g)
	//def kepler(a, e, NEWTON_G):
	// #periastron
	//	 #angular momentum
	//	
	//	return[Particle(rmin / 2., 0, 0, 0, 0, v / 2., "A"), \
	//	Particle(-rmin / 2., 0, 0, 0, 0, -v / 2., "B")]

}










