#include <mpi.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) cerr << "Run with at least 2 processes.\n";
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    const int N = 10000;
    vector<float> A(N, static_cast<float>(rank));
    vector<float> B(N, 0.0f);

    int right = (rank + 1) % size;
    int left  = (rank - 1 + size) % size;
    const int TAG = 0;

    MPI_Sendrecv(
        A.data(), N, MPI_FLOAT, right, TAG,
        B.data(), N, MPI_FLOAT, left,  TAG, 
        MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );

    cout << fixed << setprecision(2)
              << "I am process ranked " << rank
              << " and I have received b[0] = " << B[0] << '\n';

    MPI_Finalize();
    return 0;
}
