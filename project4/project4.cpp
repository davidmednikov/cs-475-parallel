#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <xmmintrin.h>

#define SSE_WIDTH		4

#ifndef SIZE
#define SIZE 1024
#endif

#ifndef NUMTRIES
#define NUMTRIES 1
#endif

float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}

float A[SIZE];
float B[SIZE];
float C[SIZE];

int
main( )
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

	// initialize the arrays:
	for( int i = 0; i < SIZE; i++ )
	{
		A[ i ] = 1.;
		B[ i ] = 2.;
	}

        double maxMegaMults = 0.;
        double getSumOnce = 0;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double sum = 0;

                double time0 = omp_get_wtime( );

                for( int i = 0; i < SIZE; i++ )
                {
                        sum += A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
                if (megaMults > maxMegaMults) {
                        maxMegaMults = megaMults;
                }
                if (t == 0) {
                        getSumOnce = sum;
                }
        }

        double maxSimdMegaMults = 0.;
        double getSimdSumOnce = 0;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                double sum = SimdMulSum(A, B, SIZE);

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
                if (megaMults > maxSimdMegaMults) {
                        maxSimdMegaMults = megaMults;
                }
                if (t == 0) {
                        getSimdSumOnce = sum;
                }
        }

        printf("%d\t%d\t%5.2lf\t%5.2lf\t%5.2lf\t%5.2lf\n", SIZE, NUMTRIES, maxMegaMults, maxSimdMegaMults, getSumOnce, getSimdSumOnce);

	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

        return 0;
}