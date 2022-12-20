#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <x86intrin.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#define THREAD_NUM 4
using namespace std;

double func(const int* vec1, const int* vec2, size_t n, int index, int step) {
    double result = 0;
    for (int i = index; i < index + step; i++)
        result += pow(vec1[i] - vec2[i], 2);
    return result;
}

int main(int argc, char **argv){

    omp_set_num_threads(THREAD_NUM);

    if (argc < 2)
        exit(1);
    int n = atoi(argv[1]);
    
    printf("Vector size: %d\n", n);

    int* vec1 = (int*) malloc(n * sizeof(int));
    int* vec2 = (int*) malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++){
        vec1[i] = i + 1;
        vec2[i] = i + 2;
    }

    double result = 0;    
    int step = n / THREAD_NUM;

    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    #pragma omp parallel for
    for (int i = 0; i < THREAD_NUM; i++)
    {
       // if (i == THREAD_NUM - 1)
       //     result += func(vec1, vec2, n, i * step, n - i * step);
       // else
            result += func(vec1, vec2, n, i * step, step);
    }
    chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
    
    printf("Result OpenMP(): %f", sqrt(result));
    printf("\n");
    printf("Time OpenMP(): %llu", chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
    printf("\n");

    return 0;
}
