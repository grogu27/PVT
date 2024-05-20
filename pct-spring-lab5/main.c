#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define CLOCK_MONOTONIC 1
int size = 1000000000;
int threshold = 10000;

double speadup(double time_nomp, double time_omp) {
    return time_nomp / time_omp;
}

double wtime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

void write(double S, int n) {
    FILE *f;
    f = fopen("res2.txt", "a");
    if (f == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%d %f\n", n, S);
    fclose(f);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void partition(int *v, int low, int high, int* pi, int* pj) {
    int i = low;
    int j = high;
    int pivot = v[(low + high) / 2];

    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j) {
            swap(&v[i], &v[j]);
            i++;
            j--;
        }
    }
    *pi = i;
    *pj = j;
}

void quicksort_serial(int *v, int low, int high) {
    if (low < high) {
        int i, j;
        partition(v, low, high, &i, &j);

        if (low < j) {
            quicksort_serial(v, low, j);
        }
        if (i < high) {
            quicksort_serial(v, i, high);
        }
    }
}

void quicksort_tasks(int *v, int low, int high) {
    int i, j;
    partition(v, low, high, &i, &j);
    if (high - low < threshold || (j - low < threshold || high - i < threshold)) {
        if (low < j)
            quicksort_tasks(v, low, j);
        if (i < high)
            quicksort_tasks(v, i, high);
    } else {
        #pragma omp task untied
        quicksort_tasks(v, low, j);
        quicksort_tasks(v, i, high);
    }
}

void fillArrayWithRandomValues(int *arr) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000 + 1;
    }
}

int main() {
    int *arr = (int*) malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    fillArrayWithRandomValues(arr);

    double t = wtime();
    quicksort_serial(arr, 0, size - 1);
    t = wtime() - t;
    printf("Serial time: %f \n", t);

    for (int i = 2; i <= 8; i += 2) {
        double time_omp;
        printf("------------%d------------\n", i);
        fillArrayWithRandomValues(arr);
        time_omp = wtime();
        #pragma omp parallel num_threads(i)
        {
            #pragma omp single
            quicksort_tasks(arr, 0, size - 1);
        }
        time_omp = wtime() - time_omp;
        printf("Tasks time: %f \n", time_omp);
        printf("Speedup: %.6f\n\n", speadup(t, time_omp));
        write(speadup(t, time_omp), i);
    }

    free(arr);
    return 0;
}
