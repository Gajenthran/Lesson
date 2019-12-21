// sort-mpi.cpp

#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>

std::vector<int> random_vector(int n) {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<int> dis;
  std::vector<int> vec;
  vec.reserve(n);
  for(int i = 0; i < n; ++i) {
    auto num = dis(gen);
    vec.push_back(num);
  }
  return vec;
}

void qsort_worker(int rank, int nb_proc, int h, int w) {
    int begin = (rank - 1) * h / (nb_proc - 1);
    int end = (rank == nb_proc - 1) ? h : begin + h / (nb_proc - 1);
    std::vector<float> buffer;
    for(int r = begin; r < end; ++r) {
        float y = -(float(r) - h / 2) * 4 / h;
        for(int c = 0; c < w; ++c) {
            float x = (float(c) - w / 2) * 4 / w;
            buffer.push_back(julia_pix(x, y));
        }
    }
    MPI_Send(buffer.data(), buffer.size(), MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
}

void qsort_master(int nb_proc, ) {
    MPI_Status status;
    for (int rank = 1; rank < nb_proc; ++rank) {
        int begin = (rank - 1) * out.h / (nb_proc - 1);
        MPI_Recv(out[begin], out.h * out.w, MPI_FLOAT, rank, 0, MPI_COMM_WORLD, &status);
    } 
}

auto qsort_worker = [](int nb_proc, auto begin, auto end) {
    auto it = begin(vec);
    for(int rank = 1; rank < nb_proc; ++rank) {
        it = it + vec.size() / nb_threads;
    }
    std::uniform_int_distribution<int> distribution(1, 100);
  for(auto it = begin; it != end; ++it) {
        *it = distribution(generator);
  }
};

int main(int argc, char* argv[]) {
    MPI_Init(&argc,&argv);
    std::vector<int> vec = random_vector(1 << 24);

    int rank, nb_proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);

    if(rank == 0) {
        qsort_master(nb_proc, );
    } else {
        qsort_worker(rank, nb_proc, );
    }

    MPI_Finalize();
    return 0;
}