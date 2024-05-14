#include "../include/include.h"

void calculate_forces_stream_storage(struct particle *p, struct particle *f[], float *m, int n)
{
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    for (int i = 0; i < n; i++) {
        f[tid][i].x = 0;
        f[tid][i].y = 0;
        f[tid][i].z = 0;
    }
    #pragma omp for schedule(dynamic, 8)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            float dist = sqrtf(powf(p[i].x - p[j].x, 2) + powf(p[i].y - p[j].y, 2) + powf(p[i].z - p[j].z, 2));
            float mag = (G * m[i] * m[j]) / powf(dist, 2);
            struct particle dir = { .x = p[j].x - p[i].x, .y = p[j].y - p[i].y, .z = p[j].z - p[i].z };
                f[tid][i].x += mag * dir.x / dist;
                f[tid][i].y += mag * dir.y / dist;
                f[tid][i].z += mag * dir.z / dist;
                f[tid][j].x -= mag * dir.x / dist;
                f[tid][j].y -= mag * dir.y / dist;
                f[tid][j].z -= mag * dir.z / dist;
        }
    } // barrier
    #pragma omp single // Итоговый вектор сил сформируем в первой строке – f[0][i]
    {
        for (int i = 0; i < n; i++) {
            for (int tid = 1; tid < nthreads; tid++) {
                f[0][i].x += f[tid][i].x;
                f[0][i].y += f[tid][i].y;
                f[0][i].z += f[tid][i].z;
            }
        }
    }
} //локальная копия вектора сил для каждого

void move_particles_stream_storage(struct particle *p, struct particle *f[], struct particle *v, float *m, int n, double dt)
{
    #pragma omp for
    for (int i = 0; i < n; i++) {
        struct particle dv = {
            .x = f[0][i].x / m[i] * dt,
            .y = f[0][i].y / m[i] * dt,
            .z = f[0][i].z / m[i] * dt,
        };
        struct particle dp = {
            .x = (v[i].x + dv.x / 2) * dt,
            .y = (v[i].y + dv.y / 2) * dt,
            .z = (v[i].z + dv.z / 2) * dt,
        };
        v[i].x += dv.x;
        v[i].y += dv.y;
        v[i].z += dv.z;
        p[i].x += dp.x;
        p[i].y += dp.y;
        p[i].z += dp.z;
    }
}

void write_stream_storage(double S, int n){ 
  FILE *f; 
  f = fopen("../graphs/res5.txt", "a"); 
  fprintf(f, "%d %f\n", n, S); 
  fclose(f); 
} 

int run_stream_storage(int i)
{
    double ttotal;
    ttotal = wtime();
    int n = 80;
    struct particle *p = malloc(sizeof(*p) * n); // Положение частиц (x, y, z)
    struct particle *f[omp_get_max_threads()];
    for (int i = 0; i < omp_get_max_threads(); i++)
        f[i] = malloc(sizeof(struct particle) * n);
    struct particle *v = malloc(sizeof(*v) * n); // Скорость частицы (x, y, z)
    float *m = malloc(sizeof(*m) * n); // Масса частицы
    for (int i = 0; i < n; i++) {
        p[i].x = rand() / (float)RAND_MAX - 0.5;
        p[i].y = rand() / (float)RAND_MAX - 0.5;
        p[i].z = rand() / (float)RAND_MAX - 0.5;
        v[i].x = rand() / (float)RAND_MAX - 0.5;
        v[i].y = rand() / (float)RAND_MAX - 0.5;
        v[i].z = rand() / (float)RAND_MAX - 0.5;
        m[i] = rand() / (float)RAND_MAX * 10 + 0.01;
    }
    double dt = 1e-5;
    #pragma omp parallel num_threads(i)
    {
        for (double t = 0; t <= 1; t += dt) { // Цикл по времени (модельному)
            calculate_forces_stream_storage(p, f, m, n); // Вычисление сил – O(N^2)
            move_particles_stream_storage(p, f, v, m, n, dt); // Перемещение тел O(N)
        }
    }
    ttotal = wtime() - ttotal;
    printf("# NBody (n=%d)\n", n);
    printf("# Elapsed time (sec): ttotal %.6f\n",ttotal);
    free(m);
    free(v);
    free(p);
    return ttotal;
}