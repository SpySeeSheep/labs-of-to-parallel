#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <chrono>
#include <time.h>
#include <iomanip>
#include <iostream>

double f(double x)
{
	//return pow(x, 4.0) - 4 * pow(x, 3.0) + 9 * pow(x, 2.0) - 16 * x;
	return 2*pow(x, 3.0) - 7 * x + 4;
}
using namespace std;
int main(int &argc, char* argv[])
{
	srand(time(NULL));
	#define N 1'000'000
	double a = 4.0, b = 5.0;
	double step = (b - a) / N;
	double sum = 0;
	auto begin = chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i)
	{
		sum += step * f(a + step * i);
	}
	auto end = chrono::high_resolution_clock::now();
	cout << "(posl)res: " << sum << " :: " <<setw(10)<< chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << "nsec\n";
	sum = 0;
	begin = chrono::high_resolution_clock::now();
	#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < N; ++i)
	{
		sum += step * f(a + step * i);
	}

	end = chrono::high_resolution_clock::now();
	cout << "(par)res: " << sum << " :: " << setw(10) << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << "nsec\n";
}	
//clock_t start = clock();
//clock_t end = clock() - start;
	//printf("%f, time: %f", sum,((double) end / CLOCKS_PER_SEC));
//// Option 1: time ~0.019 sec (N == 1'000'000, 12 threads processor)
//#pragma omp parallel 
//{
//	#pragma omp for reduction(+:sum)
//	for (int i = 0; i < N; ++i)
//	{
//		sum += step * f(a + step * i);
//	}
//}
// Option 2: time ~0.016 sec (N == 1'000'000, 12 threads processor)