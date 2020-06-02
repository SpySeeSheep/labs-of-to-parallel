#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "tbb/tick_count.h"
#include <chrono>

using namespace tbb;
using namespace std;

class gauss_method
{
public:
	double** arr;
	int i;
	const int rows = 500, cols = rows + 1;
	gauss_method()	// инициализирует матрицу размером 500:501
	{
		arr = new double* [rows];
		for (int i = 0; i < rows; i++)
		{
			arr[i] = new double[cols];
			for (int j = 0; j < cols; j++)
				arr[i][j] = double(rand() % 1000) / 100;
		}
	}

	void operator() (const blocked_range<int>& r) const		// параллельная часть алгоритма
	{
		for (int j = r.begin(); j < r.end(); j++)
		{
			double t = arr[j][i] / arr[i][i];
			for (int k = i; k < cols; k++)
				arr[j][k] -= t * arr[i][k];
		}
	}

	int copy(gauss_method d) // копирует матрицу
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				arr[i][j] = d.arr[i][j];
		}
		return 0;
	}

	double* solve()	// решение приведенной к треугольному виду
	{
		double* sol = new double[rows];
		for (int i = rows - 1; i >= 0; i--)
		{
			double sum = arr[i][cols - 1];
			for (int j = 1; j < rows - i; j++)
				sum -= arr[i][cols - 1 - j] * sol[rows - j];
			sol[i] = sum / arr[i][i];
		}
		return sol;
	}

	void print_r(double* res) {
		for (int i = 0; i < rows; i++) {
			std::cout << "x[" << i << "] = " << res[i];
		}
	}

	void solve_without_parallel()	// гаусс без паралельности
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = i + 1; j < rows; j++)
			{
				double t = arr[j][i] / arr[i][i];
				for (int k = i; k < cols; k++)
					arr[j][k] -= t * arr[i][k];
			}
		}
		double * res  =	solve();
		//print_r(res);
		delete res;
	}

};

int main(int argc, char* argv[])
{
	const int arr[10] = { 1, 3, 6, 9, 12, 100, 150, 300, 500, 1000 };	// массив масштабирования
	const int k = 1000;
	tbb::task_scheduler_init init;
	
	
	gauss_method s[11];
	// создание одинаковых матриц
	for (int i = 1; i < 11; ++i) {
		s[i].copy(s[0]);
	}

	int row = s[0].rows;
	for (int p = 0; p < 10; p++)
	{
		auto beg = chrono::high_resolution_clock::now();
		for (int j = 0; j < row; j++)
		{
			s[p].i = j;
			parallel_for(blocked_range<int>(j + 1, row, arr[p]), s[p]);
			s[p].solve();
		}
		auto end = chrono::high_resolution_clock::now();
		auto time = chrono::duration_cast<chrono::microseconds>(end - beg).count();
		cout << "Runtime parallel on " << arr[p] << " grain: " << time << "mcs\n";
	}

	auto beg = chrono::high_resolution_clock::now();
	s[10].solve_without_parallel();
	auto end = chrono::high_resolution_clock::now();
	auto time = chrono::duration_cast<chrono::microseconds>(end - beg).count();
	cout << "Runtime parallel without parallel: " << time << "mcs\n";

	return 0;
}