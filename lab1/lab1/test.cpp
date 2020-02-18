#include <iostream>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <ctime>

using namespace std;

#define llong long long
#define SIZE 10'000'000

llong** create(int, int);

void runtime_sum_array(llong* arr, long size)
{
	llong res = 0;
	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int i = 0; i < size; i++)
	{
		if (arr[i] > 0)
		{
			res += arr[i];
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime == " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << "   res ==" << res << endl;
}

void part_one() {
	srand(time(NULL));
	llong *arr = new llong[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		arr[i] = (llong)rand() % 101 - 50;
	}

	runtime_sum_array(arr, SIZE);

	sort(arr, arr + SIZE);

	runtime_sum_array(arr, SIZE);
}

llong** random(llong** arr, int str, int col)
{
	if (arr == nullptr) arr = create(str, col);
	srand(time(NULL));
	for (int i = 0; i < str; i++)
	{
		for (int j = 0; j < col; j++)
		{
			arr[i][j] = rand() % 11;
		}
	}
	return arr;
}

llong** zero(llong** arr, int str, int col)
{
	if (arr == nullptr) arr = create(str, col);
	for (int i = 0; i < str; i++)
	{
		for (int j = 0; j < col; j++)
		{
			arr[i][j] = 0;
		}
	}
	return arr;
}

llong** create(int str, int col)
{
	llong** a = new llong * [str];
	for (int i = 0; i < str; i++)
	{
		a[i] = new llong[col];
	}
	return a;
}

void multy_ijk(llong** a, int str1, int col1,
	llong** b, int str2, int col2)
{
	if (str2 != col1) return;
	llong** res = nullptr;
	res = zero(res, str1, col2);

	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int i = 0; i < str1; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			for (int k = 0; k < str2; k++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime (ijk)== " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << endl;
	/*for (int i = 0; i < str1; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			cout << res[i][j] << "  ";
		}
		cout << endl;
	}*/
}

void multy_ikj(llong** a, int str1, int col1,
	llong** b, int str2, int col2)
{
	if (str2 != col1) return;
	llong** res = nullptr;
	res = zero(res, str1, col2);

	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int i = 0; i < str1; i++)
	{
		for (int k = 0; k < str2; k++)
		{
			for (int j = 0; j < col2; j++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime (ikj)== " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << endl;
	/*for (int i = 0; i < str1; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			cout << res[i][j] << "  ";
		}
		cout << endl;
	}*/
}

void multy_kji(llong** a, int str1, int col1,
	llong** b, int str2, int col2)
{
	if (str2 != col1) return;
	llong** res = nullptr;
	res = zero(res, str1, col2);

	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int k = 0; k < str2; k++)
	{
		for (int j = 0; j < col2; j++)
		{
			for (int i = 0; i < str1; i++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime (kji)== " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << endl;
	/*for (int i = 0; i < str1; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			cout << res[i][j] << "  ";
		}
		cout << endl;
	}*/
}

void multy_kij(llong** a, int str1, int col1,
	llong** b, int str2, int col2)
{
	if (str2 != col1) return;
	llong** res = nullptr;
	res = zero(res, str1, col2);

	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int j = 0; j < col2; j++)
	{
		for (int i = 0; i < str1; i++)
		{
			for (int k = 0; k < str2; k++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime (kij)== " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << endl;
	//for (int i = 0; i < str1; i++)
	//{
	//	for (int j = 0; j < col2; j++)
	//	{
	//		cout << res[i][j] << "  ";
	//	}
	//	cout << endl;
	//}
}

void multy_jik(llong** a, int str1, int col1,
	llong** b, int str2, int col2)
{
	if (str2 != col1) return;
	llong** res = nullptr;
	res = zero(res, str1, col2);

	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int j = 0; j < col2; j++)
	{
		for (int i = 0; i < str1; i++)
		{
			for (int k = 0; k < str2; k++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime (jik)== " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << endl;
	//for (int i = 0; i < str1; i++)
	//{
	//	for (int j = 0; j < col2; j++)
	//	{
	//		cout << res[i][j] << "  ";
	//	}
	//	cout << endl;
	//}
}

void multy_jki(llong** a, int str1, int col1,
	llong** b, int str2, int col2)
{
	if (str2 != col1) return;
	llong** res = nullptr;
	res = zero(res, str1, col2);

	chrono::time_point<chrono::high_resolution_clock> beg = chrono::high_resolution_clock::now();
	for (int j = 0; j < col2; j++)
	{
		for (int k = 0; k < str2; k++)
		{
			for (int i = 0; i < str1; i++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
	cout << "runtime (jki)== " << chrono::duration_cast<chrono::microseconds>(end - beg).count() << endl;
	//for (int i = 0; i < str1; i++)
	//{
	//	for (int j = 0; j < col2; j++)
	//	{
	//		cout << res[i][j] << "  ";
	//	}
	//	cout << endl;
	//}
}


void part_two()
{
	cout << endl << endl;
	int str1 = 2000;
	int col1 = 500;
	int str2 = col1;
	int col2 = 2000;
	llong** a = create(str1, col1);
	llong** b = create(str2, col2);
	a = random(a, str1, col1);
	b = random(b, str2, col2); // [0...10]

	multy_ijk(a, str1, col1, b, str2, col2);
	multy_ikj(a, str1, col1, b, str2, col2);
	multy_kji(a, str1, col1, b, str2, col2);
	multy_kij(a, str1, col1, b, str2, col2);
	multy_jik(a, str1, col1, b, str2, col2);
	multy_jki(a, str1, col1, b, str2, col2);
}

int main()
{
	part_one();
	part_two();
}