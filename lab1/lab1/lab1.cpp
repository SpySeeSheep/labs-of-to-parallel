// lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <algorithm>
#include <iostream>

double* create_array(size_t &count)
{
	try {
		srand(time(NULL));
		double* arr = new double[count];
		
		//#pragma omp parallel for
		for (size_t i = 0; i < count; ++i)
		{
			arr[i] = (double)( rand() % 101 ) - 50.0 ; // генерация чисел от -50 до 50
		}

		return arr;
	}
	catch (...)
	{
		printf("error!");
		return nullptr;
	}
}

double** create_array(const size_t& str, const size_t& col)
{
	double** nw = new double* [str];
	for (int i = 0; i < str; ++i)
	{
		nw[i] = new double[col];
	}
	return nw;
}

double sum_arr(const double* arr, const size_t& size)
{
	double sum = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (arr[i] > 0.0)
		{
			sum += arr[i];
		}
	}
	return sum;
}

double* get_array(const double* arr, const size_t& size)
{
	double* nw = new double[size];
	for (int i = 0; i < size; ++i)
	{
		nw[i] = arr[i];
	}
	return nw;
}

void sum_plus_elelent_without_sort(const double* arr, const size_t& size)
{
	auto begin = std::chrono::high_resolution_clock::now();
	double sum = sum_arr(arr, size);
	auto end = std::chrono::high_resolution_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	printf("sum == %f, time == %I64d\n\n", sum, time);
}

void sum_plus_element_with_sort_notime(const double* arr, const size_t& size)
{
	double* nw = get_array(arr, size);
	std::sort(nw, nw + size);
	auto begin = std::chrono::high_resolution_clock::now();
	double sum = sum_arr(arr, size);
	auto end = std::chrono::high_resolution_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	printf("sum == %f, time == %I64d\n\n", sum, time);
}

void multy_matrix(const double** a, const size_t& str_a,const size_t& col_a, 
	const double** b, const size_t& str_b, const size_t& col_b)
{
	double** res = create_array(str_a, col_b);
}

int main(int argc, char **argv)
{
	printf("Part 1\n");
	size_t size = 10'000'000;
	double* arr = create_array(size);

	// Сумма положительных элементов без сортировки
	sum_plus_elelent_without_sort(arr, size);
	// Сумма положительных элементов с сортировкой (время измеряется с сортировкой)
	sum_plus_element_with_sort_notime(arr, size);

	printf("Part 2\n");

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