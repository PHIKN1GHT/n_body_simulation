#include "quadtree.h"
#include <iostream>
using namespace std;

void evolve_quadtree_serial(const std::vector<particleptr>& pars) {
	quadnode root = quadnode(-box_size, box_size, -box_size, box_size, "rt");
	for (auto iter = pars.begin(); iter != pars.end(); iter++)
		root.add(*(*iter));

	//std::vector<particleptr> allpars = root.all();
	std::vector<particleptr> allpars = pars;
	for (auto iter = allpars.begin(); iter != allpars.end(); iter++) {
		std::queue<nodeptr> nodes({ &root });
		while (nodes.size()) {
			nodeptr cur = nodes.front(); nodes.pop();
			if ((*cur).test(*(*iter))) {
				if ((*cur).npar > 0) (*cur).update(*(*iter));
			} else {
				if ((*cur).child[0] != NULL) nodes.push((*cur).child[0]);
				if ((*cur).child[1] != NULL) nodes.push((*cur).child[1]);
				if ((*cur).child[2] != NULL) nodes.push((*cur).child[2]);
				if ((*cur).child[3] != NULL) nodes.push((*cur).child[3]);
			}
		}
	}
	for (auto iter = allpars.begin(); iter != allpars.end(); iter++) (*(*iter)).drift();
}

void evolve_quadtree_parallel(const std::vector<particleptr>& pars) {
	quadnode root = quadnode(-box_size, box_size, -box_size, box_size, "rt");
	for (auto iter = pars.begin(); iter != pars.end(); iter++)
		root.add_parallel(*(*iter), 0);

	#pragma omp parallel
	{
		//std::vector<particleptr> allpars = root.all_parallel();
		std::vector<particleptr> allpars = pars;
		#pragma omp for
		for (int i = 0; i < allpars.size(); i++) {
			std::queue<nodeptr> nodes({ &root });
			while (nodes.size()) {
				nodeptr cur = nodes.front(); nodes.pop();
				if ((*cur).test(*(allpars[i]))) {
					if ((*cur).npar > 0) (*cur).update(*(allpars[i]));
				} else {
					for (int i = 0;i < 4;i++)
						if ((*cur).child[i] != NULL) nodes.push((*cur).child[i]);
				}
			}
		}
		#pragma omp for
		for (int i = 0; i < allpars.size(); i++)  (*allpars[i]).drift();
	}
}





