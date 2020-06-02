#include <mpi.h>
#include <random>
#include <iostream>

void init_matrix(float* pMatrix, int Size) {
	std::default_random_engine generator;
	std::normal_distribution<double> randomize(-10, 100);
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++)
			pMatrix[i * Size + j] = randomize(generator);
	}
}


int main(int argc, char* argv[]) {
	int size_matrix = 5000;

	double begin;
	double end;

	int rank;
	int count_process;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &count_process);

	int current_num_row = size_matrix / count_process;

	float* matrix = new float[size_matrix * size_matrix];
	init_matrix(matrix, size_matrix);

	float* current_matrix = new float[size_matrix * current_num_row];

	MPI_Scatter(matrix, size_matrix * current_num_row, MPI_FLOAT, current_matrix,
		size_matrix * current_num_row, MPI_FLOAT, 0, MPI_COMM_WORLD);

	float* rows = new float[size_matrix * current_num_row];

	if (rank == 0) {
		begin = MPI_Wtime();
	}

	for (int i = 0; i < (rank * current_num_row); i++) {
		MPI_Bcast(rows, size_matrix, MPI_FLOAT, i / current_num_row, MPI_COMM_WORLD);

		for (int j = 0; j < current_num_row; j++) {
			for (int k = i + 1; k < size_matrix; k++) {
				current_matrix[j * size_matrix + k] -= current_matrix[j * size_matrix + i] * rows[k];
			}
			current_matrix[j * size_matrix + i] = 0;
		}
	}

	int pivot_colum;
	for (int i = 0; i < current_num_row; i++) {
		pivot_colum = rank * current_num_row + i;
		for (int j = pivot_colum; j < size_matrix; j++) {
			current_matrix[i * size_matrix + j] /= current_matrix[i * size_matrix + pivot_colum];
		}

		current_matrix[i * size_matrix + pivot_colum] = 1;

		memcpy(rows, current_matrix + (i * size_matrix), size_matrix * sizeof(float));

		MPI_Bcast(rows, size_matrix, MPI_FLOAT, rank, MPI_COMM_WORLD);

		for (int j = i + 1; j < current_num_row; j++) {
			for (int k = pivot_colum + 1; k < size_matrix; k++) {
				current_matrix[j * size_matrix + k] -= current_matrix[j * size_matrix + i] * rows[k];
			}
			current_matrix[j * size_matrix + pivot_colum] = 0;
		}
	}

	for (int i = rank * current_num_row + 1; i < size_matrix; i++) {
		MPI_Bcast(rows, size_matrix, MPI_FLOAT, i / current_num_row, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
		end = MPI_Wtime() - begin;
	}

	MPI_Gather(current_matrix, size_matrix * current_num_row, MPI_FLOAT, matrix,
		size_matrix * current_num_row, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		std::cout << "Size = " << size_matrix << "\nTime: " << end << "second" << std::endl;
	}

	MPI_Finalize();

	

	delete[] matrix;
	delete[] current_matrix;
	delete[] rows;
}
