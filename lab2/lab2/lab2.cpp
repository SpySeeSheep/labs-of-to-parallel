#include <iostream>
#include <random>
#include <chrono>

using namespace std;

double** get_triangle_matrix(double** matr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        double curr = matr[i][i];
        for (int k = i + 1; k < size; k++)
        {
            double del = matr[k][i] / curr;
            for (int j = i; j < size + 1; j++)
            {
                matr[k][j] -= matr[i][j] * del;
            }
        }
    }
    return matr;
}

void print_m(double** matr, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << matr[i][j] << "   ";
        }
        cout << " | " << matr[i][size] << endl;
    }
}

void print_r(double* res, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << "x" <<i+1<<" = "<<res[i] << endl;
    }
}

double* get_result_slau(double** nmatr, int size)
{
    double* res = new double[size];
    for (int i = 0; i < size - 1; i++)
    {
        res[i] = 0;
    }
    res[size - 1] = nmatr[size - 1][size] / nmatr[size - 1][size - 1];
    for (int i = size - 2; i >= 0; i--)
    {
        double crisp = 0;
        for (int j = i + 1; j < size; j++)
        {
            crisp += nmatr[i][j] * res[j];
        }
        res[i] = (nmatr[i][size] - crisp) / nmatr[i][i];
    }
    return res;
}

bool isLiveResult(double** nmatr, int size)
{
    for (int i = 0; i < size; i++)
    {
        bool flag = false;
        for (int j = 0; j < size; j++)
        {
            if (nmatr[i][j] != 0) {
                flag = true; break;
            }
        }
        if (!flag) return false;
    }
    return true;
}

int main()
{
    int size_slau = 10000; //количество переменных и уравнений слау
    double** matrix = new double* [size_slau];
    for (int i = 0; i < size_slau; ++i)
    {
        matrix[i] = new double[size_slau + 1];
    }

    default_random_engine generator;
    tr1::normal_distribution<double> randomize(-10, 100);
    //// tests
    //double a1[3][3] = { {2, -1, -6},{7, -4, 2},{1, -2, -4} };
    //double b[3] = {-3, 13, -1};
    for (int i = 0; i < size_slau; ++i)
    {
        for (int j = 0; j < size_slau + 1; ++j)
        {
            matrix[i][j] = randomize(generator);
        }
    }
    auto beg = chrono::high_resolution_clock::now();
    double** nmatr = get_triangle_matrix(matrix, size_slau);
    //print_m(nmatr, size_slau);
    if (isLiveResult(nmatr, size_slau))
    {
        double* result = get_result_slau(nmatr, size_slau);
        print_r(result, size_slau);
        delete result;
    }
    else
    {
        cout << "Matrix is not union!"; // слау не совместная
    }

    for (int i = 0; i < size_slau; i++)
    {
        delete matrix[i];
    }
    delete matrix;
    
    auto end = chrono::high_resolution_clock::now();
    cout << "runtime == " << chrono::duration_cast<chrono::microseconds>(end - beg).count();
}