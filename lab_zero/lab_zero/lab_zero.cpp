#include <stdio.h>
#include <math.h>
#include <omp.h>
//#include <chrono>
#include <time.h>

double f(double x)
{
	return pow(x, 4.0) - 4 * pow(x, 3.0) + 9 * pow(x, 2.0) - 16 * x;
}

int main(int &argc, char* argv[])
{
	clock_t start = clock();
	double a = 4.0, b = 5.0;
	
	#define N 1'000'000'000
	double step = (b - a) / N;
	double sum = 0;
	//// Option 1: time ~0.019 sec
	//#pragma omp parallel 
	//{
	//	#pragma omp for reduction(+:sum)
	//	for (int i = 0; i < N; ++i)
	//	{
	//		sum += step * f(a + step * i);
	//	}
	//}

	// Option 2: time ~0.016 sec
	#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < N; ++i)
	{
		sum += step * f(a + step * i);
	}
	clock_t end = clock() - start;
	printf("%f, time: %f", sum,((double) end / CLOCKS_PER_SEC));
}