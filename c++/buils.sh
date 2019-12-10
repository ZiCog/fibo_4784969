#!/bin/bash

g++ -Wall -O3 -o fibo_karatsuba fibo_karatsuba.cpp fibo.cpp

g++ -Wall -O3 -DUSE_OMP -fopenmp -o fibo_karatsuba fibo_karatsuba.cpp fibo.cpp



