#include <iostream>
#include <mutex>
#include <vector>
#include <random>
#include <mpi.h>

void multyply(
	std::vector<std::vector<double>>& res,		// Результат манипулирования данными
	std::vector<std::vector<double>> const* a,	// Первая матрица
	std::vector<std::vector<double>> const* b,	// Вторая матрица
	int curr_str,		// Текущее начальное положение в строке
	int curr_col,		// Текущее начальное положение в столбике
	int end_str,		// Текущее конечное положение в строке
	int end_col)		// Текущее конечное положение в столбике
{
	for (int i = curr_str; i <= end_str; ++i) {
		for (int j = curr_col; j <= end_col; ++j){
			for (int k = 0; k < b->size(); ++k) {
				res[i][j] += a->operator[](i)[k] * b->operator[](k)[j];
			}
		}
	}
}

void init_vector(std::vector<std::vector<double>>& vec, int const& str, int const& col) {
	std::default_random_engine generator;
	std::normal_distribution<double> randomize(-10, 100);
	for (int i = 0; i < str; i++) {
		std::vector<double> tmp;
		for (int j = 0; j < col; j++) {
			tmp.push_back(randomize(generator));
		}
		vec.push_back(tmp);
	}
}

void resize_vector(std::vector<std::vector<double>>& vec, int str, int col) {
	for (int i = 0; i < vec.size(); i++) {
		vec[i].resize(col);
	}
	vec.resize(str);
}

int main(int argc, char ** argv)
{
	int const str_a = 10000;
	int const col_a = 500;
	int const str_b = 500;
	int const col_b = 10000;

	std::vector<std::vector<double>> a;
	std::vector<std::vector<double>> b;
	init_vector(a, str_a, col_a);
	init_vector(b, str_b, col_b);
	std::vector<std::vector<double>> res;
	for (int i = 0; i < str_a; ++i) {
		std::vector<double> vec;
		for (int j = 0; j < col_b; ++j) {
			vec.push_back(0);
		}
		res.push_back(vec);
	}


	int mpi_count_proc,		// количество процессов
		mpi_init_status,	// статус инициализации процессов
		mpi_current_rank;	// номер текущего процесса
	
	int E,					// нагрузка
		n = str_a,			// количество новых строк
		p = col_b,			// количество новых столбиков
		curr_str = -1,			// положение текущего процесса в матрице (№строки)
		curr_col = -1,			// положение текущего процесса в матрице (№колонки)	
		end_str = -1 ,			// конечное положение по номеру строки
		end_col = -1 ;			// конечное положение по номеру колонки


	// Инициализация MPI
	if ((mpi_init_status = MPI_Init(&argc, &argv)) != 0) {
		return EXIT_FAILURE;
	}

	MPI_Comm_size(MPI_COMM_WORLD, &mpi_count_proc);		// получение числа процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_current_rank);	// получение текущего процесса

	// выделение памяти под каждую матрицу в каждом процессе.
	resize_vector(a, a.size(), a[0].size());
	resize_vector(b, b.size(), b[0].size());
	resize_vector(res, res.size(), res[0].size());

	// Передача векторов другим процессам
	for (int i = 0; i < a.size(); i++) {
		MPI_Bcast(a[i].data(), a[i].size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}
	for (int i = 0; i < b.size(); i++) {
		MPI_Bcast(b[i].data(), b[i].size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}
	for (int i = 0; i < res.size(); i++) {
		MPI_Bcast(res[i].data(), res[i].size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}

	// определение начала и конца рассчета для текущего процесса
	E = n * p / mpi_count_proc;
	if (E == 0) {
		if (mpi_current_rank < n * p) {
			curr_str = mpi_current_rank / p;
			curr_col = mpi_current_rank % p;
			end_str = (mpi_current_rank + 1) / p;
			end_col = (mpi_current_rank + 1) % p;
		}
	}
	else {
		curr_str = (mpi_current_rank * E) / p;
		curr_col = (mpi_current_rank * E) % p;
		end_str = (mpi_current_rank * E + E - 1) / p;
		end_col = (mpi_current_rank * E + E - 1) % p;
	}


	if (mpi_current_rank == mpi_count_proc - 1) {
		end_str = n - 1;
		end_col = p - 1;
	}

	// начало измерения потоконезависимого таймера
	double beg = MPI_Wtime();
	try {
		// выполнение алгоритма
		multyply(res, &a, &b, curr_str, curr_col, end_str, end_col);
	}
	catch (std::exception err) {
		std::cerr << "Процесс №" << mpi_current_rank << " незадействуется!";
	}

	// синхронизация потоков
	MPI_Barrier(MPI_COMM_WORLD);


	// сбор результата
	double** result = new double* [res.size()];
	for (int i = 0; i < res.size(); i++) {
		result[i] = new double[res[i].size()];
	}
	for (int i = 0; i < res.size(); i++) {
		MPI_Reduce(res[i].data(), result[i], res[i].size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	}

	double end = MPI_Wtime() - beg;
	if (mpi_current_rank == 0) {
		std::cout << "Runtime: " << end << " sec. \n";
	}
	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
