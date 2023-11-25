#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank and size of the MPI communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Print the rank and size
    printf("Hello from process %d of %d\n", rank, size);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
