#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <math.h>
#include <immintrin.h>
#include <x86intrin.h>
using namespace std;

#pragma intrinsic(__rdtsc)

double withoutParallel(const int* vec1, const int* vec2, size_t n) {
	double underSqrt = 0;
	for (int i = 0; i < n; i++)
		underSqrt += pow(vec1[i] - vec2[i], 2);
	return sqrt(underSqrt);
}

int main(int argc, char **argv) {
    if (argc < 2)
       exit(1);
    int n = atoi(argv[1]);
    
    printf("Vector size: %d\n", n);
    
    int* vec1 = (int*) malloc(n * sizeof(int));
    int* vec2 = (int*) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
       vec1[i] = i + 1;
       vec2[i] = i + 2;
    }


    unsigned long long int start_time, end_time;

    start_time = __rdtsc();
    double result = withoutParallel(vec1, vec2, n);
    end_time = __rdtsc();
    printf("Result withoutParallel(): %f", result);
    printf("\n");
    printf("Time withoutParallel(): %llu", end_time - start_time);
    printf("\n");

    return 0;
}
