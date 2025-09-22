#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) cerr << "This program requires exactly 2 MPI processes.\n";
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    const int N = 10000;
    const int TAG = 0;
    int other = (rank == 0) ? 1 : 0;

    vector<float> A(N, static_cast<float>(rank));
    vector<float> B(N, 0.0f);

    if (rank == 0) {
        MPI_Send(A.data(), N, MPI_FLOAT, other, TAG, MPI_COMM_WORLD);
        MPI_Recv(B.data(), N, MPI_FLOAT, other, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(B.data(), N, MPI_FLOAT, other, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(A.data(), N, MPI_FLOAT, other, TAG, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "Rank " << rank << " received = " << B[0]<<endl;

    MPI_Finalize();
    return 0;
}
