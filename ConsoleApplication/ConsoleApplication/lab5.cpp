#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>

std::vector<double> get_result(std::vector<std::vector<double>> triangle_mat) {
    std::vector<double> res;
    res.resize(triangle_mat.size());
    for (int i = triangle_mat.size() - 1; i >= 0; i--)
    {
        double sum = 0;
        for (int j = i + 1; j < triangle_mat.size() - 1; j++) {
            sum += triangle_mat[i][j] * res[j];
        }
        res[i] = (triangle_mat[i][triangle_mat[0].size() - 1] - sum) / triangle_mat[i][i];
    }
    return res;
}

int main(int argc, char** argv)
{
    int const row = 5;
    int const colomn = row + 1;
    std::vector<std::vector<double>> a;

    std::default_random_engine generator;
    std::normal_distribution<double> randomize(-10, 100);

    for (int i = 0; i < row; ++i) {
        std::vector<double> vec;
        for (int j = 0; j < colomn; j++) {
            vec.push_back(randomize(generator));
        }
        a.push_back(vec);
    }
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            std::cout << a[i][j] << "  ";
        }
        std::cout << "\n";
    }

    int init_status, rank, proc;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < a.size(); i++) {
        MPI_Bcast(a[i].data(), a[i].size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // параллельное приведение к треугольному виду

    MPI_Barrier(MPI_COMM_WORLD);

    std::vector<std::vector<double>> result;
    result.resize(a.size());
    for (int i = 0; i < a.size(); ++i) {
        result[i].resize(a[i].size());
    }
    for (int i = 0; i < a.size(); i++) {
        MPI_Reduce(a[i].data(), result[i].data(), a[i].size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        std::vector<double> res = get_result(a);
        for (int i = 0; i < res.size(); i++) {
            std::cout << res[i] << "\n";
        }
    }
    MPI_Finalize();


}