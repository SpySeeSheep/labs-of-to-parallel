// lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std::chrono;

double* create_array(size_t& count)
{
	try {
		srand(time(NULL));
		double* arr = new double[count];

		//#pragma omp parallel for
		for (size_t i = 0; i < count; ++i)
		{
			arr[i] = (double)(rand() % 101) - 50.0; // генерация чисел от -50 до 50
		}

		return arr;
	}
	catch (...)
	{
		printf("error!");
		return nullptr;
	}
}
double** create_array(size_t str, size_t col)
{
	double** nw = new double* [str];
	for (size_t i = 0; i < str; ++i)
	{
		nw[i] = new double[col];
	}
	return nw;
}
void random_input(double**& arr, size_t str, size_t col)
{
	for (size_t i = 0; i < str; ++i)
	{
		for (size_t j = 0; j < col; ++j)
		{
			arr[i][j] = rand() % 100;
		}
	}
}
void clear(double** arr, size_t str, size_t col)
{
	for (size_t i = 0; i < str; ++i)
	{
		delete arr[i];
	}
	delete arr;
}
void ini_zero_value(double**& arr, const size_t& str, const size_t& col)
{
	for (size_t i = 0; i < str; ++i)
	{
		for (size_t j = 0; j < col; ++j)
		{
			arr[i][j] = 0;
		}
	}
}
//void array_in_file(std::fstream& out, double** arr, const size_t& str, const size_t& col)
//{
//	for (size_t i = 0; i < str; ++i)
//	{
//		for (size_t j = 0; j < col; ++j)
//		{
//			out << arr[i][j] << "  ";
//		}
//		out << std::endl;
//	}
//	out << std::endl;
//}

//double sum_arr(const double* arr, const size_t& size)
//{
//	double sum = 0;
//	for (size_t i = 0; i < size; ++i)
//	{
//		if (arr[i] > 0.0)
//		{
//			sum += arr[i];
//		}
//	}
//	return sum;
//}

double* get_array(const double* arr, const size_t& size)
{
	double* nw = new double[size];
	for (size_t i = 0; i < size; ++i)
	{
		nw[i] = arr[i];
	}
	return nw;
}

void sum_plus_elelent_without_sort(const double* arr, const size_t& size)
{
	double sum = 0;

	clock_t begin = clock();
	for (size_t i = 0; i < size; ++i)
	{
		if (arr[i] > 0.0)
		{
			sum += arr[i];
		}
	}
	clock_t end = clock();

	double time = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("sum == %f, time == %f\n\n", sum, time);
}
void sum_plus_element_with_sort_notime(const double* arr, const size_t& size)
{
	double* nw = get_array(arr, size);
	std::sort(nw, nw + size);
	double sum = 0;

	auto begin = clock();
	for (size_t i = 0; i < size; ++i)
	{
		if (arr[i] > 0.0)
		{
			sum += arr[i];
		}
	}
	auto end = clock();

	double time = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("sum == %f, time == %f\n\n", sum, time);
}


void multy_matrix_ijk(double** a, const size_t& str_a, const size_t& col_a,
	double** b, const size_t& str_b, const size_t& col_b)
{
	if (col_a != str_b)	return;
	size_t str = str_a;
	size_t col = col_b;
	double** res = create_array(str, col);
	ini_zero_value(res, str, col);
	double time = 0;

	time_point<high_resolution_clock> begin = high_resolution_clock::now();
	for (size_t i = 0; i < str_a; ++i)
	{
		for (size_t j = 0; j < str_b; ++j)
		{
			for (size_t k = 0; k < str_b; ++k)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	time_point<high_resolution_clock> end = high_resolution_clock::now();
	time = duration_cast<microseconds>(end - begin).count();

	//std::fstream fout;
	//std::fstream log;
	//fout.open("result.txt", std::ios_base::out | std::ios_base::trunc);
	//array_in_file(fout, res, str, col);
	//log.open("log.txt", std::ios_base::out);
	//array_in_file(log, res, str, col);

	printf("result was load to file (\"result.txt\"). time == %f\n\n", time);
	//log.close();
	//fout.close();

}

void multy_matrix_ikj(double** a, const size_t& str_a, const size_t& col_a,
	double** b, const size_t& str_b, const size_t& col_b)
{
	if (col_a != str_b)	return;
	size_t str = str_a;
	size_t col = col_b;
	double** res = new double* [str];
	for (size_t i = 0; i < str; i++)
	{
		res[i] = new double[col];
	}
	ini_zero_value(res, str, col);
	double time = 0;

	time_point<high_resolution_clock> begin = high_resolution_clock::now();
	for (size_t i = 0; i < str_a; ++i)
	{
		for (size_t k = 0; k < str_b; ++k)
		{
			for (size_t j = 0; j < str_b; ++j)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	time_point<high_resolution_clock> end = high_resolution_clock::now();
	time = duration_cast<microseconds>(end - begin).count();
	//std::fstream fout;
	//std::fstream log;
	//fout.open("result.txt", std::ios_base::out | std::ios_base::trunc);
	//array_in_file(fout, res, str, col);
	//log.open("log.txt", std::ios_base::out);
	//array_in_file(log, res, str, col);

	printf("result was load to file (\"result.txt\"). time == %f\n\n", time);
	//log.close();
	//fout.close();
	clear(res, str, col);
}

int main(int argc, char** argv)
{
	//printf("Part 1\n");
	//size_t size = 100'000'000;
	//double* arr = create_array(size);

	//// Сумма положительных элементов без сортировки
	//sum_plus_elelent_without_sort(arr, size);
	//// Сумма положительных элементов с сортировкой (время измеряется с сортировкой)
	//sum_plus_element_with_sort_notime(arr, size);
	//delete arr;
	printf("Part 2\n");

	size_t str_x = 10;
	size_t col_x = 50;
	size_t str_y = col_x;
	size_t col_y = 5;
	// создание массивов размерностью (str x col)
	double** x = create_array(str_x, col_x);
	double** y = create_array(str_y, col_y);

	// заполнение массивов
	random_input(x, str_x, col_x);
	random_input(y, str_y, col_y);


	// test
	//double** x = create_array(str_x, col_x);
	//double** y = create_array(str_y, col_y);
	//x[0][0] = 1.0;
	//x[0][1] = 2.0;
	//x[0][2] = 3.0;
	//x[1][0] = 4.0;
	//x[1][1] = 5.0;
	//x[1][2] = 6.0;
	//y[0][0] = 1.0;
	//y[0][1] = 2.0;
	//y[1][0] = 3.0;
	//y[1][1] = 4.0;
	//y[2][0] = 5.0;
	//y[2][1] = 6.0;

	multy_matrix_ijk(x, str_x, col_x, y, str_y, col_y);
	multy_matrix_ikj(x, str_x, col_x, y, str_y, col_y);

	clear(x, str_x, col_y);
	clear(y, str_y, col_y);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.