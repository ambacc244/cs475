#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

int	NowYear;		// 2019 - 2024
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
float NowFire;

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				50.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

float
SQR( float x )
{
        return x*x;
}

float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

unsigned int seed = 0;  // a thread-private variable
float x = Ranf( &seed, -1.f, 1.f );

void GrainDeer(){
  while( NowYear < 2025 ){
  	// compute a temporary next-value for this quantity
  	// based on the current state of the simulation:
  	float height = NowHeight;
    int deer = NowNumDeer;

    if (deer > height)
			deer--;
		else if (deer < height)
			deer++;

  	// DoneComputing barrier:
  	#pragma omp barrier
  	NowNumDeer = deer;

  	// DoneAssigning barrier:
  	#pragma omp barrier

  	// DonePrinting barrier:
  	#pragma omp barrier

  }
}

void Grain(){
  while( NowYear < 2025 ){
  	// compute a temporary next-value for this quantity
  	// based on the current state of the simulation:
  	float height = NowHeight;
    float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
    float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

    height += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
    height -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

    if(height <= 0.)
      height = 0.;
  	// DoneComputing barrier:
  	#pragma omp barrier
  	NowHeight = height;

  	// DoneAssigning barrier:
  	#pragma omp barrier

  	// DonePrinting barrier:
  	#pragma omp barrier

  }
}

void MyAgent(){
  while( NowYear < 2025 ){
  	// compute a temporary next-value for this quantity
  	// based on the current state of the simulation:
  	float height = NowHeight;
    float fire = sqrt(( NowTemp-20.));
  //  printf("%f\n", ( (float) (rand_r(&seed)%10) )/5.);

    fire = fire + ( (float) (rand_r(&seed)%10) )/5.;
    fire = fire - ( (float) (rand_r(&seed)%10) )/5.;

    if(fire <= 0.)
      fire = 0.;

  	// DoneComputing barrier:
  	#pragma omp barrier
  	NowFire = fire*5.;
    //NowHeight = hight;

  	// DoneAssigning barrier:
  	#pragma omp barrier

  	// DonePrinting barrier:
  	#pragma omp barrier

  }

}

void Watcher(){
  while( NowYear < 2025 ){
  	// compute a temporary next-value for this quantity
  	// based on the current state of the simulation:

  	// DoneComputing barrier:
  	#pragma omp barrier

  	// DoneAssigning barrier:
  	#pragma omp barrier
    printf("%d\t%d\t%d\t%f\t%f\t%f\t%f\n", NowYear, NowMonth+1, NowNumDeer, (5./9.)*(NowTemp-32.), NowPrecip*2.54, NowHeight*2.54, NowFire);

    if(NowMonth == 11){
      NowMonth = 0;
      NowYear++;
    }
    else{
      NowMonth++;
    }

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );

    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
      NowPrecip = 0.;

    // DonePrinting barrier:
    #pragma omp barrier
  }

}

int main( int argc, char *argv[ ] ){

  // starting date and time:
  NowMonth =    0;
  NowYear  = 2019;

  // starting state (feel free to change this if you want):
  NowNumDeer = 1;
  NowHeight =  1.;

  float	NowPrecip = 0.;
  float	NowTemp = 20.;
  float NowFire = 0.;


  //omp_init_lock(&Lock);

  omp_set_num_threads( 4 );	// same as # of sections
//  InitBarrier(3);

  #pragma omp parallel sections
  {
  	#pragma omp section
  	{
      GrainDeer( );
    }

    #pragma omp section
    {
    	Grain( );
    }

    #pragma omp section
    {
      MyAgent( );
    }

    #pragma omp section
    {
    	Watcher( );
  	}
  }       // implied barrier -- all functions must return in order
  	// to allow any of them to get past here
}
