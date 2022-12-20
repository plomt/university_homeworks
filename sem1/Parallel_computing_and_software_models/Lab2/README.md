lab: g++ -mavx2 -march=native -o lab main.cpp -lm

lab1: g++ -mavx2 -march=native -fopenmp -o lab1 openmp.cpp -lm

lab2: g++ -mavx2 -march=native -o lab2 posix.cpp -lm

lab3: g++ -mavx2 -march=native -o lab3 win32.cpp -lm
