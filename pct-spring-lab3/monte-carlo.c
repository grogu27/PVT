#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

int THREADS = 2;

double t_serial = 0.0;
double t_parallel = 0.0;

/* pseudo-random number in the [0, 1] */
double getrand_serial() { 
  return (double)rand() / RAND_MAX; 
}

double getrand_parallel(unsigned int* seed) {
  return (double)rand_r(seed) / (RAND_MAX + 1.0);
}

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x, double y) { 
  return (x/(y*y)); 
}

const double PI = 3.14159265358979323846;
int n = 10000000;

void serial() {
  int in = 0;
  double s = 0;
  t_serial = wtime();
  for (int i = 0; i < n; i++) {
    double x = getrand_serial() ;            /* x in (0, 1) */
    double y = getrand_serial() * 3 + 2;     /* y in (2, 5) */
     if (y >= 2 && y <= 5) {
      in++;
      s += func(x, y);
    }
  }
  double v = 1 + 3;
  double res = v * s / in;
  t_serial = wtime() - t_serial;
  printf("Elapsed time (serial): %.6f sec.\n", t_serial);
  printf("Result: %.12f, n = %d\n", res, n);
}

void parallel() {
  int in = 0;
  double s = 0;
  t_parallel = wtime();
#pragma omp parallel
  {
    double s_loc = 0;
    int in_loc = 0;
    unsigned int seed = omp_get_thread_num();
#pragma omp for nowait
    for (int i = 0; i < n; i++) {
      double x = getrand_parallel(&seed) - 1; /* x in (0, 1) */
      double y = getrand_parallel(&seed);     /* y in (2, 5) */
      if (y >= 2 && y <= 5) {
        in_loc++;
        s_loc += func(x, y);
      }
    }
#pragma omp atomic
    s += s_loc;
#pragma omp atomic
    in += in_loc;
  }
  double v = 1 * 3;
  double res = v * s / in;
  t_parallel = wtime() - t_parallel;
  printf("Elapsed time (parallel): %.6f sec.\n", t_parallel);
  printf("Result: %.12f, n = %d\n", res, n);
}

int main() {
  printf("VARIANT: %d\n", 15 % 3 + 1);
  for (int i = 0; i < 2; i++) {
    n = (i > 0 ? pow(10, 8) : pow(10, 7) );
    char buff[100] = "# Threads   Speedup\n";
    char filename[30];
    sprintf(filename, "prog-monte-carlo-%d.dat", i);
    FILE* file = fopen(filename, "w");
    for (; THREADS <= 8; THREADS += 2) {
      printf("-------------10**%d-------------\n", i);
      printf("-------------%d------------- n = %d\n", THREADS, n);
      printf("> serial\n");
      serial();
      printf("> parallel\n");
      parallel();
      printf("Speedup: %f\n", t_serial / t_parallel);
      printf("\n\n");
      char tmp[20];
      sprintf(tmp, "%d\t\t%f\n", THREADS, t_serial / t_parallel);
      strcat(buff, tmp);
    }
    strcat(buff, "\0");
    fwrite(buff, sizeof(char), strlen(buff), file);
    fclose(file);
    THREADS = 2;
  }
  return 0;
}