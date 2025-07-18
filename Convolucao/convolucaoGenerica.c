#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "input_image.h"
#include "kernel_x.h"
#include "kernel_y.h"

#ifndef IMAGE_WIDTH
#error "IMAGE_WIDTH não definido em input_image.h"
#endif

#ifndef IMAGE_HEIGHT
#error "IMAGE_HEIGHT não definido em input_image.h"
#endif

#ifndef KERNEL_SIZE
#error "KERNEL_SIZE não definido em kernel_x.h"
#endif

int main()
{
    const int width = IMAGE_WIDTH;
    const int height = IMAGE_HEIGHT;
    const int ksize = KERNEL_SIZE;
    const int khalf = ksize / 2;

    const uint8_t *img = image_data;
    const int *kx = kernel_x;
    const int *ky = kernel_y;

    uint8_t result[IMAGE_WIDTH * IMAGE_HEIGHT] = {0};

    printf("Começou\n");
    fflush(stdout);

    printf("###START###\n");
    for (int y = khalf; y < height - khalf; ++y)
    {
        for (int x = khalf; x < width - khalf; ++x)
        {
            float acc_x = 0.0f, acc_y = 0.0f;

            for (int ky_i = 0; ky_i < ksize; ++ky_i)
            {
                for (int kx_i = 0; kx_i < ksize; ++kx_i)
                {
                    int dx = kx_i - khalf;
                    int dy = ky_i - khalf;
                    int px = x + dx;
                    int py = y + dy;
                    int img_idx = py * width + px;
                    int k_idx = ky_i * ksize + kx_i;

                    acc_x += kx[k_idx] * img[img_idx];
                    acc_y += ky[k_idx] * img[img_idx];
                }
            }

            float mag = sqrtf(acc_x * acc_x + acc_y * acc_y);
            result[y * width + x] = (mag > 255.0f) ? 255 : (uint8_t)mag;
        }
    }
    printf("###END###\n");

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            printf("%3d ", result[y * width + x]);
        }
        printf("\n");
    }

    return 0;
}
