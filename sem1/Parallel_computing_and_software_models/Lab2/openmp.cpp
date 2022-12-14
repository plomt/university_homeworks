#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <x86intrin.h>
#include <iostream>
#include <fstream>
#include <math.h>
#define THREAD_NUM 8
using namespace std;

#pragma intrinsic(__rdtsc)

double func(const int* vec1, const int* vec2, size_t n) {
    double underSqrt = 0;
    for (int i = 0; i < n; i++)
        underSqrt += pow(vec1[i] - vec2[i], 2);
    return underSqrt;
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

    start_time = __rdtsc();
    //omp_set_dynamic(0);
    #pragma omp parallel num_threads(THREAD_NUM)
    {
        result += func(vec1, vec2, n);
    }
    end_time = __rdtsc();
    printf("Result OpenMP(): %f", sqrt(result));
    printf("\n");
    printf("Time OpenMP(): %llu", end_time - start_time);
    printf("\n");

    return 0;
}
