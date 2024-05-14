#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

#define CLOCK_MONOTONIC		1
extern const float G;
extern omp_lock_t *locks; // Массив из N блокировок (мьютексов) — блокировка на уровне отдельных ячеек
struct particle { float x, y, z; };

double S(double time_nomp,double time_omp);
void write_serial(double S);
void move_particles(struct particle *p, struct particle *f, struct particle *v, float *m, int n, double dt);
void calculate_forces_serial(struct particle *p, struct particle *f, float *m, int n);
double wtime();
void write_one_criticle(double S, int n);
void calculate_forces_one_critical(struct particle *p, struct particle *f, float *m, int n,int i);
void write_six_atomar(double S, int n);
void move_particles_nowait(struct particle *p, struct particle *f, struct particle *v, float *m, int n, double dt);
void calculate_forces_six_atomar(struct particle *p, struct particle *f, float *m, int n);
void write_nblock(double S, int n);
void calculate_forces_nblock(struct particle *p, struct particle *f, float *m, int n);
void calculate_forces_redundant_calculations(struct particle *p, struct particle *f, float *m, int n);
void write_redundant_calculations(double S, int n);
void write_stream_storage(double S, int n);
void move_particles_stream_storage(struct particle *p, struct particle *f[], struct particle *v, float *m, int n, double dt);
void calculate_forces_stream_storage(struct particle *p, struct particle *f[], float *m, int n);
int run_one_criticle(int i);
int run_serial();
int run_nblock(int i);
int run_redundant_calculations(int i);
int run_six_atomar(int i);
int run_redundant_calculations(int i);
int run_stream_storage(int i);