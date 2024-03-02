#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
# define CLOCK_MONOTONIC		1
enum {W = 15360, H = 8640};

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

int image_is_dark(uint8_t *img, int width, int height)
{
    int count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (img[i * width + j] >= 128) {
                count++;
            }
        }
    }
    return count < width * height / 2;
}

int image_is_dark_opt(uint8_t *img, int width, int height)
{
    int count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            count += (img[i * width + j] >> 7); // shr al, 7; деление на 2^7
        }
    }
    return count < width * height / 2;
}

int main()
{
    uint8_t *image = malloc(sizeof(*image) * W * H);
    srand(0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            image[i * W + j] = rand() % 256;
        }
    }

    double t = wtime();
    int dark = image_is_dark(image, W, H);
    
    t = wtime() - t;

    printf("Time %.6f, dark %d\n", t, dark);
    
    double d = wtime();
    int dark2 = image_is_dark_opt(image, W, H);
    d = wtime() - d;
    printf("Time %.6f, dark %d\n", d, dark2);

    free(image);
    return 0;
}
