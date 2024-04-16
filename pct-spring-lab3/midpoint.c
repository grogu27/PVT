#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int THREADS = 2;

double t_serial = 0.0;
double t_parallel = 0.0;

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x) { 
    return (sin(x + 2)/(0.4 + cos(x))); 
}

void serial() {
  const double a = -1;
  const double b = 1;
  const int n0 = 100000000;
  int n = n0, k;
  const double eps = 1E-5;
  double sq[2];
  double delta = 1;
  t_serial = wtime();
  for (k = 0; delta > eps; n *= 2, k ^= 1) {
    double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; i++) s += func(a + h * (i + 0.5));
        sq[k] = s * h;
    if (n > n0) 
        delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
  }
  t_serial = wtime() - t_serial;
  printf("Elapsed time (serial): %.6f sec.\n", t_serial);
  printf("Result: %.12f\n", sq[k]);
}

void parallel() {
    const double a = -1;
    const double b = 1;
    const int n0 = 100000000;
    const double eps = 1E-5;
    printf("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0, eps);
    double sq[2]; 
    double t = omp_get_wtime();
    //t_parallel = wtime();
    #pragma omp parallel num_threads(THREADS)
    {
        int n = n0, k;
        double delta = 1;
        for (k = 0; delta > eps; n *= 2, k ^= 1) {
            double h = (b - a) / n;
            double s = 0.0;
            sq[k] = 0;
            #pragma omp barrier

            #pragma omp for nowait
            for (int i = 0; i < n; i++)
                s += func(a + h * (i + 0.5));

            #pragma omp atomic
            sq[k] += s * h;

            #pragma omp barrier
            if (n > n0)
                delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }
        #pragma omp master
        printf("n = %d\n", n);
        printf("Result: %.12f; Runge rule: EPS = %e, n = %d\n", sq[k], eps, n / 2);    
    }
        //t_parallel = wtime() - t_parallel;
        t = omp_get_wtime() - t;

        printf("Elapsed time (parallel): %.6f sec.\n", t);
        t_parallel = t;
}


int main() {
    printf("VARIANT: %d\n", 15 % 6 + 1);
    char buff[100] = "# Threads   Speedup\n";
    for (; THREADS <= 8; THREADS += 2) {
        printf("-------------%d-------------\n", THREADS);
        printf("> serial\n");
        serial();
        printf("> parallel\n");
        parallel();
        printf("Speedup: %f\n", t_serial / t_parallel);
        printf("---------------------------\n");
        char tmp[20];
        sprintf(tmp, "%d\t\t%f\n", THREADS, t_serial / t_parallel);
        strcat(buff, tmp);
        printf("\n\n");
    }
    strcat(buff, "\0");
    FILE* file = fopen("prog-midpoint.dat", "w");
    fwrite(buff, sizeof(char), strlen(buff), file);
    fclose(file);
    return 0;
}