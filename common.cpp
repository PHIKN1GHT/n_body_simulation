#include "common.h"



int MPI_RANK, MPI_SIZE;


double softensqdist(double sqdist) {
    return sqdist < 1.0 ? 1.0 : sqdist;
}
