#define _USE_MATH_DEFINES
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const double GRAIN_GROWS_PER_MONTH = 9.0;
const double TOURISTS_VISIT_PER_MONTH = 3.0;
const double ONE_DEER_EATS_PER_MONTH = 1.0;

const double AVG_PRECIP_PER_MONTH = 7.0; // average
const double AMP_PRECIP_PER_MONTH = 6.0; // plus or minus
const double RANDOM_PRECIP = 2.0;        // plus or minus noise

const double AVG_TEMP = 60.0;    // average
const double AMP_TEMP = 20.0;    // plus or minus
const double RANDOM_TEMP = 10.0; // plus or minus noise

const double MIDTEMP = 40.0;
const double MIDPRECIP = 10.0;

unsigned int seed = 0;

double getTemperature(double);
double getPrecipitation(double);
double randomDouble(unsigned int*, double, double);
int randomInt(unsigned int*, int, int);
double squareMe(double);

void grainDeer();
void grain();
void tourists();
void watcher();

int nowYear;  // 2020 - 2025
int nowMonth; // 0 - 11

double nowPrecip; // inches of rain per month
double nowTemp;   // temperature this month
double nowHeight; // grain height in inches
int nowNumDeer;   // number of deer in the current population
int nowNumTourists;

int main(int argc, char *argv[]) {

#ifndef _OPENMP
  fprintf(stderr, "No OpenMP support!\n");
  return 1;
#endif

  nowMonth = 0;
  nowYear = 2020;

  nowNumDeer = 1;
  nowHeight = 1.;

  nowNumTourists = 3;

  double ang = (30. * (double)nowMonth + 15.) * (M_PI / 180.);
  nowTemp = getTemperature(ang) + randomDouble(&seed, -RANDOM_TEMP, RANDOM_TEMP);
  nowPrecip = getPrecipitation(ang) + randomDouble(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
  if (nowPrecip < 0.) {
    nowPrecip = 0.;
  }

  while (nowYear < 2026) {
    // set the number of threads to use in the for-loop:`
    omp_set_num_threads(4);
    #pragma omp parallel sections
    {
      #pragma omp section
      {
        grainDeer();
      }

      #pragma omp section
      {
        grain();
      }

      #pragma omp section
      {
        tourists();	// your own
      }

      #pragma omp section
      {
        watcher();
      }

    } // implied barrier -- all functions must return in order
    // to allow any of them to get past here
  }
}

void grainDeer() {
  double nextNumDeer;
  if (nowNumDeer > nowHeight) {
    nextNumDeer = nowNumDeer - 1;
  }
  else if (nowNumDeer < nowHeight) {
    nextNumDeer = nowNumDeer + 1;
  } else {
    nextNumDeer = nowNumDeer;
  }
  if (nowNumDeer > nowNumTourists * 2) {
    nextNumDeer = nextNumDeer + 1;
  }
  else if (nowNumDeer < nowNumTourists * 2) {
    nextNumDeer = nextNumDeer - 1;
  }
  if (nextNumDeer < 0) {
    nextNumDeer = 0;
  }
  #pragma omp barrier
  nowNumDeer = nextNumDeer;
  #pragma omp barrier
  #pragma omp barrier
}

void grain() {
  double tempFactor = exp(-squareMe((nowTemp - MIDTEMP) / 10.));
  double precipFactor = exp(-squareMe((nowPrecip - MIDPRECIP) / 10.));
  double growth = tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
  double consumed = (double)nowNumDeer * ONE_DEER_EATS_PER_MONTH;
  double nextHeight = nowHeight + growth - consumed;
  if (nextHeight < 0.) {
    nextHeight = 0.;
  }
  #pragma omp barrier
  nowHeight = nextHeight;
  #pragma omp barrier
  #pragma omp barrier
}

void tourists() {
  double tempFactor = exp(-squareMe((nowTemp - MIDTEMP) / 10.)) * 3;
  double precipFactor = exp(-squareMe((nowPrecip - MIDPRECIP) / 10.)) / 2;
  double warmWeatherTourists = tempFactor * TOURISTS_VISIT_PER_MONTH;
  double noRainTourists = precipFactor * TOURISTS_VISIT_PER_MONTH;
  double nextNumTourists = nowNumTourists - warmWeatherTourists + noRainTourists;
  if (nextNumTourists < 0) {
    nextNumTourists = 0;
  }
  #pragma omp barrier
  nowNumTourists = nextNumTourists;
  #pragma omp barrier
  #pragma omp barrier
}

void watcher() {
  #pragma omp barrier
  #pragma omp barrier
  printf("%d\t%5.2lf\t%5.2lf\t%5.2lf\t%d\t%d\n", nowMonth + 12 * (nowYear - 2020), (5./9.)*(nowTemp -32), nowPrecip * 2.54, nowHeight * 2.54, nowNumTourists, nowNumDeer);
  nowMonth += 1;
  if (nowMonth == 12) {
    nowMonth = 0;
    nowYear += 1;
  }
  double ang = (30. * (double)nowMonth + 15.) * (M_PI / 180.);
  nowTemp = getTemperature(ang) + randomDouble(&seed, -RANDOM_TEMP, RANDOM_TEMP);
  nowPrecip = getPrecipitation(ang) + randomDouble(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
  if (nowPrecip < 0.) {
    nowPrecip = 0.;
  }
  #pragma omp barrier
}

double getTemperature(double ang) {
  return AVG_TEMP - AMP_TEMP * cos(ang);
}

double getPrecipitation(double ang) {
  return AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
}

double randomDouble(unsigned int *seedp, double low, double high) {
  double r = (double)rand_r(seedp); // 0 - RAND_MAX
  return (low + r * (high - low) / (double)RAND_MAX);
}

int randomInt(unsigned int *seedp, int ilow, int ihigh) {
  double low = (double)ilow;
  double high = (double)ihigh + 0.9999f;
  return (int)(randomDouble(seedp, low, high));
}

double squareMe(double x) {
  return x * x;
}