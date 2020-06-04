#include "quadtree.h"
#include <mpi.h>
#include <iostream>
#include <string>
#include <ctime>
bool output = false;
//bool output = true;
using namespace std;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MPI_RANK);
	MPI_Comm_size(MPI_COMM_WORLD, &MPI_SIZE);
	omp_set_dynamic(0);
	omp_set_num_threads(NUM_THREAD);

	clock_t time_start, time_end;
	MPI_MASTER_COND time_start = clock();
	srand(21231);
	auto particles = random_sample(NUM_PAR);// = dual_kepler();
	for (int i = 0; i < 8; i++) {
		MPI_MASTER_COND {
			if (output) {
				for (auto par : particles)
					cout << (*par).x << " " << (*par).y << " ";
				cout << endl;
			}
		}
		evolve_bruteforce_serial(particles);
		evolve_bruteforce_parallel(particles);
		evolve_quadtree_parallel(particles);
		evolve_quadtree_serial(particles);
		
	}

	MPI_Finalize();
	MPI_MASTER_COND {
		time_end = clock();
		cout << "time costs: " << 1000 * (time_end - time_start) / (double)CLOCKS_PER_SEC << " ms" << endl;
	}
	
	return 0;
}






