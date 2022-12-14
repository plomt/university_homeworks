#include<stdio.h>
#include<stdlib.h>
#include<immintrin.h>
#include<math.h>
#include<x86intrin.h>
#include<stdint.h>

#pragma intrinsic(__rdtsc)

double withoutParallel(const int* vec1, const int* vec2, size_t n) {
    /* The length of the difference of two vectors without parallel methods */

    double underSqrt = 0;
    for (int i = 0; i < n; i++)
        underSqrt += pow(vec1[i] - vec2[i], 2);
    return sqrt(underSqrt);
}

double withIntrinsics(const int* vec1, const int* vec2, size_t n) {
    size_t i, j;
    double result = 0;

    for (i = 0; i < n; i += 8) {
        __m256i a = _mm256_set_epi32(vec1[i], vec1[i + 1], vec1[i + 2], vec1[i + 3], vec1[i + 4], vec1[i + 5], vec1[i + 6], vec1[i + 7]);
        __m256i b = _mm256_set_epi32(vec2[i], vec2[i + 1], vec2[i + 2], vec2[i + 3], vec2[i + 4], vec2[i + 5], vec2[i + 6], vec2[i + 7]);

        __m256i sub_container = _mm256_sub_epi32(a, b);
        __m256i pow_container = _mm256_mullo_epi32(sub_container, sub_container);

        int* f = (int*)&pow_container;
        for (j = 0; j < 8; j += 1)
            result += f[j];
    }

    return sqrt(result);
}

double withVectorExtensions(const int* vec1, const int* vec2, size_t n) {
    typedef int v8sa __attribute__ ((vector_size(32)));
    size_t i, j;
    double result = 0;

    for (i = 0; i < n; i += 8) {
        v8sa vector1 = {vec1[i], vec1[i + 1], vec1[i + 2], vec1[i + 3], vec1[i + 4], vec1[i + 5], vec1[i + 6], vec1[i + 7]};
        v8sa vector2 = {vec2[i], vec2[i + 1], vec2[i + 2], vec2[i + 3], vec2[i + 4], vec2[i + 5], vec2[i + 6], vec2[i + 7]};

        v8sa sub_container = vector1 - vector2;
        v8sa pow_container = sub_container * sub_container;

        int* f = (int*)&pow_container;
        for (j = 0; j < 8; j += 1)
            result += f[j];
    }
    return sqrt(result);
}

int main(int argc, char **argv) {
    /* input data */
    if (argc < 2)
        exit(1);
    int n = atoi(argv[1]);

    printf("Vector size: %d\n", n);

    int add = n % 8;
    int* a1; int* a2;

    if (add) {
        a1 = (int*) malloc(n * (sizeof(int)));
        a2 = (int*) malloc(n  * (sizeof(int)));
    }
    else {
        a1 = (int*) malloc((n + (8 - (add))) * (sizeof(int)));
        a2 = (int*) malloc((n + (8 - (add))) * (sizeof(int)));
    }

    for (int i = 0; i < n; ++i)
    {
        a1[i] = i + 1;

        a2[i] = i + 2;
    }

    if (add) {
        for (int i = n; i < n + (8 - (add)); i++) {
            a1[i] = 0;
            a2[i] = 0;
        }
    }
    n = n + (8 - (add));

//    for (int i = 0; i < n; i++) {
//        printf("a1[%d] = ", i);
//        scanf("%d", &a1[i]);
//    }
//    printf("Input values the second vector:\n");
//    for (int i = 0; i < n; i++) {
//        printf("a2[%d] = ", i);
//        scanf("%d", &a2[i]);
//    }

//    printf("Vector a1: ");
//    for (int i = 0; i < n; i++)
//        printf("%d ", a1[i]);
//    printf("\n");
//    printf("Vector a2: ");
//    for (int i = 0; i < n; i++)
//        printf("%d ", a2[i]);
//    printf("\n");

    unsigned long long int start_time, end_time;

    /// START 1.withoutParallel
    start_time = __rdtsc();
    double result = withoutParallel(a1, a2, n);
    end_time = __rdtsc();
    printf("Result withoutParallel(): %f", result);
    printf("\n");
    printf("time withoutParallel(): %llu", end_time - start_time);
    printf("\n");
    /// END 1.withoutParallel

    /// START 2.withIntrinsics
    start_time = __rdtsc();
    double result1 = withIntrinsics(a1, a2, n);
    end_time = __rdtsc();
    printf("Result withIntrinsics(): %f", result1);
    printf("\n");
    printf("time withIntrinsics(): %llu", end_time - start_time);
    printf("\n");
    /// END 2.withIntrinsics

    /// START 3.withVectorExtensions
    start_time = __rdtsc();
    double result2 = withVectorExtensions(a1, a2, n);
    end_time = __rdtsc();
    printf("Result withVectorExtensions(): %f", result2);
    printf("\n");
    printf("time withVectorExtensions(): %llu", end_time - start_time);
    printf("\n");
    /// END 3.withVectorExtensions

    return 0;
}
