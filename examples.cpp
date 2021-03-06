#include "common.h"
#include <vector>

std::vector<particleptr> kepler() {
	std::vector<particleptr> particles;
	double a = 10, e = 0.; double rmin = a * (1 - e), h = sqrt((rmin * (1 + e) * newton_g * 2)),
		v = h / rmin;
	particles.push_back(new particle({ (double)(rmin / 2.), (double)0, (double)0,  (v / 2)  , 1 }));
	particles.push_back(new particle({ (double)(-rmin / 2.), (double)0, (double)0, (-v / 2) , 1 }));
	return particles;
}

std::vector<particleptr> dual_kepler() {
	std::vector<particleptr> particles;
	double a = 10, e = 0.; double rmin = a * (1 - e), h = sqrt((rmin * (1 + e) * newton_g * 2)),
		v = h / rmin;
	particles.push_back(new particle({ (double)(rmin / 2.), (double)0, (double)0,  (v / 2)  , 1 }));
	particles.push_back(new particle({ (double)(-rmin / 2.), (double)0, (double)0, (-v / 2) , 1 }));
	a = 30; rmin = a * (1 - e); h = sqrt((rmin * (1 + e) * 9 * newton_g * 2)); v = h / rmin;
	particles.push_back(new particle({ (double)(rmin / 2.), (double)0, (double)0,  (v / 2)  , 1 }));
	particles.push_back(new particle({ (double)(-rmin / 2.), (double)0, (double)0,  (-v / 2)  , 1 }));
	return particles;
}


#define rand_in(a, b) (double)((rand() / double(RAND_MAX)) * (b - a + 1) + a)

std::vector<particleptr> random_sample(int n) {
	std::vector<particleptr> particles;
	for (int i=0; i<n;i++)
		particles.push_back(new particle({ rand_in(-15, 15), rand_in(-15, 15), rand_in(-1, 1),  rand_in(-1, 1), rand_in(2, 5) }));
	return particles;
}
