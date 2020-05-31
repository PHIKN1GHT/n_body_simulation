#include "quadtree.h"
#include <mpi.h>
#include <iostream>
#include <string>
#include <ctime>


using namespace std;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MPI_RANK);
	MPI_Comm_size(MPI_COMM_WORLD, &MPI_SIZE);
	clock_t time_start, time_end;
	MPI_MASTER_COND time_start = clock();

//if (argc > 1 && arg[1] == '')
//	for (int i = 0; i < argc; i++)
//		cout << i << " -> " << argv[i] << endl;
	//if (argc > 1) cout << 1 << endl;
	//for(int i=0;i<argc;i++)
	//	if (string(argv[i])) == 
	//int rnk = 0, siz = 0; MPI_Init(&argc, &argv);
	srand(233);
	auto particles = random_sample(23);// = dual_kepler();
	//auto particles = dual_kepler();


	
	//MPI_Bcast(&particles, sizeof(particles), MPI_BYTE, 0, MPI_COMM_WORLD);

	//tree.addall(particles);
	for (int i = 0; i < 5; i++) {
		MPI_MASTER_COND {
			for (auto par : particles)
				cout << (*par).x << " " << (*par).y << " ";
			cout << endl;
		}
			evolve_quadtree_serial(particles);
			//evolve_bruteforce_serial(particles);
		//evolve_bruteforce_parallel(particles);
	}

	MPI_Finalize();
	MPI_MASTER_COND {
		time_end = clock();
		cout << "time costs: " << 1000 * (time_end - time_start) / (double)CLOCKS_PER_SEC << " ms" << endl;
	}
	
	return 0;
}






