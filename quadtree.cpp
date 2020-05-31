
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
		
		//std::cout << "print tree" << std::endl;
		//root.printtree();
	}

	for (auto iter = allpars.begin(); iter != allpars.end(); iter++) (*(*iter)).drift();
}








