
#include "quadtree.h"
#include <iostream>
using namespace std;

std::vector<particleptr> kepler2() {
	std::vector<particleptr> particles;
	double a = 10, e = 0.; double rmin = a * (1 - e), h = sqrt((rmin * (1 + e) * newton_g * 2)),
		v = h / rmin;
	particles.push_back(new particle({ {(double)(rmin / 2.), (double)0}, {(double)0,  (v / 2) } , 1 }));
	particles.push_back(new particle({ {(double)(-rmin / 2.), (double)0}, {(double)0, (-v / 2)} , 1 }));
	return particles;
}

std::vector<particleptr> kepler4() {
	std::vector<particleptr> particles;
	double a = 10, e = 0.; double rmin = a * (1 - e), h = sqrt((rmin * (1 + e) * newton_g * 2)),
		v = h / rmin;
	particles.push_back(new particle({ {(double)(rmin / 2.), (double)0}, {(double)0,  (v / 2) } , 1 }));
	particles.push_back(new particle({ {(double)(-rmin / 2.), (double)0}, {(double)0, (-v / 2)} , 1 }));
	a = 30; rmin = a * (1 - e); h = sqrt((rmin * (1 + e) * 9 * newton_g * 2)); v = h / rmin;
	particles.push_back(new particle({ {(double)(rmin / 2.), (double)0}, {(double)0,  (v / 2) } , 1 }));
	particles.push_back(new particle({ {(double)(-rmin / 2.), (double)0}, {(double)0,  (-v / 2) } , 1 }));
	return particles;
}

void test_quad_tree(){
	//cout << "hello, world" << endl;

	
	
	//particle p1 = particle({ {(double)(rmin / 2.), (double)0}, {(double)0,  (v / 2) } , 1});
	//particle p2 = particle({ {(double)(-rmin / 2.), (double)0}, {(double)0, (-v / 2)} , 1});
	//particle p3 = particle({ {(double)0, (double)(rmin / 2.)}, {(v / 2), (double)0} , 1 });
	//particle p4 = particle({ {(double)0, (double)(-rmin / 2.)}, {(-v / 2), (double)0} , 1 });
	//particles.push_back(&p1); particles.push_back(&p2);

	//// kepler2:


	////

	auto particles = kepler4();

	quadtree tree;
	//tree.addall(particles);
	for (int i = 0; i < 3000; i++) {
		for (auto par : particles)
			cout << (*par).current.x << " " << (*par).current.y << " ";
		cout << endl;

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










