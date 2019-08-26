#include "simd.p4.h"

#ifndef NUMTRIALS
#define NUMTRIALS 100
#endif

#ifndef SIZE
#define SIZE 20000
#endif

void Mul(float *A, float *B, float *C, int size) {
  for(int i = 0; i < size; i++)
    C[i] = A[i] * B[i];
}

float MulSum(float *A, float *B, int size) {
  float sum = 0.;
  for(int i = 0; i < size; i++)
    sum = sum + A[i] * B[i];

  return sum;
}

float Ranf( float low, float high ){
  float r = (float) rand();               // 0 - RAND_MAX
  float t = r  /  (float) RAND_MAX;       // 0. - 1.

  return low + t * ( high - low );
}

int main(int argc, char const *argv[]) {

  double s_time;
  double f_time;
  double simd_time;
  double mul_time;
  double simd_max_time;
  double mul_max_time;
  double sum;

  float *A = new float[SIZE];
	float *B = new float[SIZE];
	float *C = new float[SIZE];

  for(int i = 0; i < SIZE; i++){
    A[i] = Ranf( 0., 2.);
    B[i] = Ranf( 2., 4.);
  }

  for(int i = 0; i < NUMTRIALS; i++){
    s_time = omp_get_wtime();
    SimdMul(A, B, C, SIZE);
    f_time = omp_get_wtime();

    simd_time = (double)SIZE / (f_time - s_time) / 1000000.;

    if(simd_max_time < simd_time)
      simd_max_time = simd_time;
  }

  for(int i = 0; i < NUMTRIALS; i++){
    s_time = omp_get_wtime();
    Mul(A, B, C,SIZE);
    f_time = omp_get_wtime();

    mul_time = (double)SIZE / (f_time - s_time) / 1000000.;

    if(mul_max_time < mul_time)
      mul_max_time = mul_time;

  }

  printf("%d\t%.5f\t%.5f\t%.5f\n", SIZE, simd_max_time/mul_max_time, simd_max_time, mul_max_time);

  return 0;
}
