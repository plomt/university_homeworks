#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <x86intrin.h>
#include <iostream>
#include <fstream>
#include <math.h>
#define THREAD_NUM 6
using namespace std;

#pragma intrinsic(__rdtsc)

double func(const int* vec1, const int* vec2, size_t n, int index, int step) {
    double result = 0;
    for (int i = index; i < index + step; i++)
        result += pow(vec1[i] - vec2[i], 2);
    return result;
}

int main(int argc, char **argv){
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
    unsigned long long int start_time, end_time;
    int step = n / THREAD_NUM;

    start_time = __rdtsc();
    #pragma omp parallel for
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (i == THREAD_NUM - 1)
            result += func(vec1, vec2, n, i * step, n - i * step);
        else
            result += func(vec1, vec2, n, i * step, step);
    }
    end_time = __rdtsc();
    printf("Result OpenMP(): %f", sqrt(result));
    printf("\n");
    printf("Time OpenMP(): %llu", end_time - start_time);
    printf("\n");

    return 0;
}
