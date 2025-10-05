#!/bin/bash



g++ -std=c++17 -Wall -O3 -o fibo_karatsuba fibo_karatsuba.cpp fibo.cpp 

g++ -std=c++17 -Wall -O3 -DUSE_OMP -fopenmp -o fibo_karatsuba_omp fibo_karatsuba.cpp fibo.cpp



