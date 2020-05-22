#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "tbb/tick_count.h"
#include <chrono>

using namespace tbb;
using namespace std;

class Resh
{
public:
	double** arr;
	int i;
	const int row = 500, col = row + 1;
	Resh()	// инициализирует матрицу размером 500:501
	{
		arr = new double* [row];
		for (int i = 0; i < row; i++)
		{
			arr[i] = new double[col];
			for (int j = 0; j < col; j++)
				arr[i][j] = double(rand() % 1000) / 100;
		}
	}

	void operator() (const blocked_range<int>& r) const		// параллельная часть алгоритма
	{
		for (int j = r.begin(); j < r.end(); j++)
		{
			double t = arr[j][i] / arr[i][i];
			for (int k = i; k < col; k++)
				arr[j][k] -= t * arr[i][k];
		}
	}

	int copy(Resh d) // копирует матрицу
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				arr[i][j] = d.arr[i][j];
		}
		return 0;
	}

	double* sol()	// решение приведенной к треугольному виду
	{
		double* sol = new double[row];
		for (int i = row - 1; i >= 0; i--)
		{
			double sum = arr[i][col - 1];
			for (int j = 1; j < row - i; j++)
				sum -= arr[i][col - 1 - j] * sol[row - j];
			sol[i] = sum / arr[i][i];
		}
		return sol;
	}

	void print_r(double* res) {
		for (int i = 0; i < row; i++) {
			std::cout << "x[" << i << "] = " << res[i];
		}
	}

	void gau()	// гаусс без паралельности
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = i + 1; j < row; j++)
			{
				double t = arr[j][i] / arr[i][i];
				for (int k = i; k < col; k++)
					arr[j][k] -= t * arr[i][k];
			}
		}
		double * res  =	sol();
		//print_r(res);
	}

};

int main(int argc, char* argv[])
{
	const int arr[9] = { 1, 3, 6, 9, 12, 100, 150, 500, 1000 };	// массив масштабирования
	const int k = 1000;
	tbb::task_scheduler_init init;
	
	
	Resh s[10];
	// создание одинаковых матриц
	for (int i = 1; i < 10; ++i) {
		s[i].copy(s[0]);
	}
	auto beg = chrono::high_resolution_clock::now();
	s[0].gau();
	auto end = chrono::high_resolution_clock::now();
	auto time = chrono::duration_cast<chrono::microseconds>(end - beg).count();
	cout << "Runtime without parallel: " << time << "mcs\n";

	int row = s[0].row;
	for (int p = 1; p < 10; p++)
	{
		beg = chrono::high_resolution_clock::now();
		for (int j = 0; j < row; j++)
		{
			s[p].i = j;
			parallel_for(blocked_range<int>(j + 1, row, arr[p - 1]), s[p]);
			s[p].sol();
		}
		end = chrono::high_resolution_clock::now();
		time = chrono::duration_cast<chrono::microseconds>(end - beg).count();
		cout << "Runtime parallel on " << arr[p - 1] << " grain: " << time << "mcs\n";
	}
	
	return 0;
}