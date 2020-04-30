#define _USE_MATH_DEFINES
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// setting the number of threads:
#ifndef NUMT
#define NUMT 1
#endif

// setting the number of nodes:
#ifndef NUMNODES
#define NUMNODES 8
#endif

#define XMIN -1.
#define XMAX 1.
#define YMIN -1.
#define YMAX 1.

double getHeight(int, int);

int main(int argc, char *argv[]) {

#ifndef _OPENMP
  fprintf(stderr, "No OpenMP support!\n");
  return 1;
#endif

  // set the number of threads to use in the for-loop:`
  omp_set_num_threads(NUMT);

  // the area of a single full-sized tile:
  double fullTileArea = (((XMAX - XMIN) / (double)(NUMNODES - 1)) *
                        ((YMAX - YMIN) / (double)(NUMNODES - 1)));

  double time0 = omp_get_wtime();

  double totalVolume = 0.0;
  #pragma omp parallel for default(none) shared(fullTileArea) reduction(+:totalVolume)
  for( int i = 0; i < NUMNODES*NUMNODES; i++ )
  {
    int iu = i % NUMNODES;
    int iv = i / NUMNODES;
    double height = getHeight( iu, iv );
    double volume = height * fullTileArea * 2;

    if (iu == 0 || iu == NUMNODES - 1) {
      if (iv == 0 || iv == NUMNODES - 1) {
        volume *= (double) 0.25;
      } else {
        volume *= (double) 0.5;
      }
    } else if (iv == 0 || iv == NUMNODES - 1) {
      volume *= (double) 0.5;
    }

    totalVolume += volume;
  }

  double time1 = omp_get_wtime();
  double MegaHeights  = (double)NUMNODES*NUMNODES / (time1 - time0) / 1000000.;

  printf("%d\t", NUMT);
  printf("%d\t", NUMNODES);
  printf("%5.2lf\t", MegaHeights);
  printf("%5.2lf\n", totalVolume);
}

// iu,iv = 0 .. NUMNODES-1
double getHeight(int iu, int iv) {
	double x = -1.  +  2.*(double)iu /(double)(NUMNODES-1);	// -1. to +1.
	double y = -1.  +  2.*(double)iv /(double)(NUMNODES-1);	// -1. to +1.

	double xn = pow( fabs(x), (double) 4 );
	double yn = pow( fabs(y), (double) 4 );
	double r = 1. - xn - yn;
	if( r < 0. )
	        return 0.;
	double height = pow( 1. - xn - yn, 1./(double) 4 );
	return height;
}