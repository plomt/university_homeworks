#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <fstream>
#include <math.h>

#define MAX_THREADS 4
#define BUF_SIZE 255

#pragma intrinsic(__rdtsc)

DWORD WINAPI MyThreadFunction( LPVOID lpParam );
void ErrorHandler(LPCTSTR lpszFunction);

typedef struct thread_data {
    int* vec1;
    int* vec2;
    size_t n;
    int index;
    int step;
    double result;
} threadData_t, *PthreadData_t;

int _tmain(int argc, char **argv) {
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
    int step = n / MAX_THREADS;
    PthreadData_t pDataArray[MAX_THREADS];
    DWORD dwThreadIdArray[MAX_THREADS];
    HANDLE hThreadArray[MAX_THREADS];

   for (int i = 0; i < MAX_THREADS; i++) {
      pDataArray[i] = (PthreadData_t) HeadAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(threadData_t));

      if (pDataArray[i] == NULL)
         ExitProcess(2);

      pDataArray[i]->vec1 = vec1;
      pDataArray[i]->vec2 = vec2;
      pDataArray[i]->n = n;
      pDataArray[i]->index = i * step;
      pDataArray[i]->step = (i == MAX_THREADS - 1) ? n - pDataArray[i]->index : step;
      pDataArray[i]->result = 0;

      hThreadArray[i] = CreateThread(NULL, 0, MyThreadFunction, pDataArray[i], 0, &dwThreadIdArray[i]);

      if (hThreadArray[i] == NULL) {
         ErrorHandler(TEXT("CreateThread"));
         ExitProcess(3);
      }
   }

   WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

   for (int i = 0; i < MAX_THREADS, i++) {
      CloseHandle(hThreadArray[i]);
      if (pDataArray[i] != NULL) {
         HeapFree(GetProcessHeap(), 0, pDataArray[i]);
         pDataArray[i] = NULL;
      }
   }  
   
   return 0;
}

DWORD WINAPI MyThreadFunction( LPVOID lpParam ) {
   HANDLE hStdout;
   PthreadData_t pDataArray;

   TCHAR msgBuf[BUF_SIZE];
   size_t cchStringSize;
   DWORD dwChars;

   hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
   if ( hStdout == INVALID_HANDLE_VALUE )
      return 1;

   pDataArray = (PthreadData_t)lpParam;

   pDataArray->result = 0;
   for (int i = pDataArray->index; i < (pDataArray->index + pDataArray->step); i++)
      pDataArray->result += pow(pDataArray->vec1[i] - pDataArray->vec2[i], 2);

   return 0;
}

void ErrorHandler(LPCTSTR lpszFunction) {
   LPVOID lpMsgBuf;
   LPVOID lpDisplayBuf;
   DWORD dw = GetLastError();

   FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      dw,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR) &lpMsgBuf,
      0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR), lpMsgBuf) + lstrlen((LPCTSTR), lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK);

   LocalFree(lpMsgBuff);
   LocalFree(lpDisplayBuf);
}


