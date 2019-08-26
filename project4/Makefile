all: arraymult

simd.p4.o:	simd.p4.h  simd.p4.cpp
		g++-8  -c  simd.p4.cpp -o simd.p4.o

arraymult:	arraymult.cpp simd.p4.o
		g++-8  -o arraymult  arraymult.cpp simd.p4.o  -lm  -fopenmp
