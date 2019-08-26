#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT1	         1
#define NUMT4	         4

#define ARRAYSIZE      10	// you decide
#define NUMTRIES       10	// you decide

float A1[ARRAYSIZE];
float B1[ARRAYSIZE];
float C1[ARRAYSIZE];

float A4[ARRAYSIZE];
float B4[ARRAYSIZE];
float C4[ARRAYSIZE];

int main( ){
  #ifndef _OPENMP
          fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
          return 1;
  #endif

        omp_set_num_threads( NUMT1 );
        fprintf( stderr, "Using %d threads\n", NUMT1 );

        double maxMegaMults1 = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < ARRAYSIZE; i++ )
                {
                        C1[i] = A1[i] * B1[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults1 )
                        maxMegaMults1 = megaMults;
        }

        printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults1 );

        omp_set_num_threads( NUMT4 );
        fprintf( stderr, "Using %d threads\n", NUMT4 );

        double maxMegaMults4 = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < ARRAYSIZE; i++ )
                {
                        C4[i] = A4[i] * B4[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults4 )
                        maxMegaMults4 = megaMults;
        }

        printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults4 );

        double S = maxMegaMults1/maxMegaMults4;
        printf( "4-thread-to-one-thread speedup = %8.2lf \n", S );

        float Fp = (4./3.)*( 1. - (1./S) );
        printf( "Parallel Fraction = %8.2lf \n", Fp );


	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

        return 0;
}
