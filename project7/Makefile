all: project7

simd.o:	simd.h  simd.cpp
		g++-8  -c  simd.cpp -o simd.o

project7:	project7.cpp simd.o
		g++-8  -o project7  project7.cpp simd.o CL/libOpenCL.so  -lm  -fopenmp
