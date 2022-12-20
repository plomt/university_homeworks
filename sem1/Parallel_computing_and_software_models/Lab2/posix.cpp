#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <math.h>
#include <x86intrin.h>

#define NUM_THREADS           6
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define BAD_MESSAGE         -13
#define SUCCESS               0

using namespace std;

typedef struct thread_data {
    int* vec1;
    int* vec2;
    size_t n;
    int index;
    int step;
    double result;
} threadData_t;

void* threadFunc(void* args) {
    threadData_t *my_data = (threadData_t*) args;

    my_data->result = 0;
    for (int i = my_data->index; i < (my_data->index + my_data->step); i++)
        my_data->result += pow(my_data->vec1[i] - my_data->vec2[i], 2);

    return SUCCESS;
}

int main(int argc, char **argv) {
    if (argc < 2)
        exit(1);
    int status;
    int status_addr;
    int n = atoi(argv[1]);

    printf("Vector size: %d\n", n);

    int* vec1 = (int*) malloc(n * sizeof(int));
    int* vec2 = (int*) malloc(n * sizeof(int));
  
    for (int i = 0; i < n; i++) {
        vec1[i] = i + 1;
        vec2[i] = i + 2;
    }

    pthread_t threads[NUM_THREADS];
    threadData_t td[NUM_THREADS];
    int step = n / NUM_THREADS;

    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
    for (int i = 0; i < NUM_THREADS; i++) {
        td[i].vec1 = vec1;
        td[i].vec2 = vec2;
        td[i].n = n;
        td[i].index = i * step;
        td[i].step = (i == NUM_THREADS - 1) ? n - td[i].index : step;
        td[i].result = 0;

        status = pthread_create(&threads[i], NULL, threadFunc, (void*)&td[i]);
        if (status != 0) {
            printf("main error: can't create thread, status = %d\n", status);
            exit(ERROR_CREATE_THREAD);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        status = pthread_join(threads[i], (void**)&status_addr);
        if (status != SUCCESS) {
             printf("main error: can't join thread, status = %d\n", status);
             exit(ERROR_JOIN_THREAD);
        }
    }

    chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
    
    double result = 0;    
    for (int i = 0; i < NUM_THREADS; i++)
        result += td[i].result;

    printf("Result Posix(): %f\n", sqrt(result));
    printf("Time Posix(): %lu\n", chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());

    return 0;
}

