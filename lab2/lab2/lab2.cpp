#include <iostream>
#include <random>
#include <chrono>

using namespace std;

// Необязательная функция!!!
// отключение асинхронного i/o в консоль
// развязывание ручного ввода, т.к. он не используется
const int ZERO = []() {
    ios_base::sync_with_stdio(false);
    /*returned before cin tie*/ cin.tie(nullptr);
    return 0;
}();

double** get_triangle_matrix(double** matr, long size)
{
    for (long i = 0; i < size - 1; i++)
    {
        double curr = matr[i][i];
        #pragma omp parallel for
        for (long k = i + 1; k < size; k++)
        {
            double del = matr[k][i] / curr;
            for (long j = i; j < size + 1; j++)
            {
                matr[k][j] -= matr[i][j] * del;
            }
        }
    }
    return matr;
}

void print_m(double** matr, long size)
{
    for (long i = 0; i < size; i++)
    {
        for (long j = 0; j < size; j++)
        {
            cout << matr[i][j] << "   ";
        }
        cout << " | " << matr[i][size] << endl;
    }
}

void print_r(double* res, long size)
{
    for (long i = 0; i < size; i++)
    {
        cout << "x" <<i+1<<" = "<<res[i] << endl;
    }
}

double* get_result_slau(double** nmatr, long size)
{
    double* res = new double[size];
    #pragma omp parallel for
    for (long i = 0; i < size - 1; i++)
    {
        res[i] = 0;
    }
    res[size - 1] = nmatr[size - 1][size] / nmatr[size - 1][size - 1];
    for (long i = size - 2; i >= 0; i--)
    {
        double crisp = 0;
        for (long j = i + 1; j < size; j++)
        {
            crisp += nmatr[i][j] * res[j];
        }
        res[i] = (nmatr[i][size] - crisp) / nmatr[i][i];
    }
    return res;
}

bool is_it_has_zero_str(double** nmatr, long size)
{
    for (long i = 0; i < size; i++)
    {
        bool flag = false;
        for (long j = 0; j < size; j++)
        {
            if (nmatr[i][j] != 0) {
                flag = true; break;
            }
        }
        if (!flag) return false;
    }
    return true;
}

void generate(double** matr, long size)
{
    default_random_engine generator;
    tr1::normal_distribution<double> randomize(-10, 100);
    for (long i = 0; i < size; ++i)
    {
        for (long j = 0; j < size + 1; ++j)
        {
            matr[i][j] = randomize(generator);
        }
    }
}

int main()
{
    long size_slau = 2000; //количество переменных и уравнений слау
    double** matrix = new double* [size_slau];
    for (long i = 0; i < size_slau; ++i)
    {
        matrix[i] = new double[size_slau + 1];
    }
    generate(matrix, size_slau);
    

    /*tests 1.*/
    //  ans == {1;-1;1}
    // size_slau = 3;
    //double a1[3][3] = { {2, -1, -6},{7, -4, 2},{1, -2, -4} };
    //double b1[3] = {-3, 13, -1};
    //for (long i = 0; i < size_slau; ++i)
    //{
    //    for (long j = 0; j < size_slau; ++j)
    //    {
    //        matrix[i][j] = a1[i][j];
    //    }
    //    matrix[i][size_slau] = b1[i];
    //}

    /*tests 2.*/
    //  ans == {1;2;2,0}
    //size_slau = 4;
    //double a2[4][4] = { {2, 5, 4, 1},{1, 3, 2, 1},{2, 10, 9, 7},{3, 8, 9, 2} };
    //double b2[4] = {20,11,40,37};
    //for (long i = 0; i < size_slau; ++i)
    //{
    //    for (long j = 0; j < size_slau; ++j)
    //    {
    //        matrix[i][j] = a2[i][j];
    //    }
    //    matrix[i][size_slau] = b2[i];
    //}

    /*tests 3.*/
    //  ans == {4, 0, -1}
    //size_slau = 3;
    //double a2[3][3] = { {1, 2, 3},{2, -1, 2},{1, 1, 5} };
    //double b2[3] = {1,6,-1};
    //for (long i = 0; i < size_slau; ++i)
    //{
    //    for (long j = 0; j < size_slau; ++j)
    //    {
    //        matrix[i][j] = a2[i][j];
    //    }
    //    matrix[i][size_slau] = b2[i];
    //}

    auto beg = chrono::high_resolution_clock::now();
    double** nmatr = get_triangle_matrix(matrix, size_slau);
    //print_m(nmatr, size_slau);
    if (is_it_has_zero_str(nmatr, size_slau))
    {
        double* result = get_result_slau(nmatr, size_slau);
        print_r(result, size_slau);
        delete result;
        auto end = chrono::high_resolution_clock::now();
        cout << "runtime == " << chrono::duration_cast<chrono::microseconds>(end - beg).count();

    }
    else
    {
        cout << "Matrix is not union!"; // слау расходится или имеет бесконечно возможные решения
        auto end = chrono::high_resolution_clock::now();
        cout << "runtime == " << chrono::duration_cast<chrono::microseconds>(end - beg).count();

    }

    for (long i = 0; i < size_slau; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    
}