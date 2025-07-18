#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "image_data.h"

#define CHANNELS 1

int sobel_x[9] = {
  -1, 0, 1,
  -2, 0, 2,
  -1, 0, 1,
};

int sobel_y[9] = {
  -1, -2, -1,
   0,  0,  0,
   1,  2,  1,
};

int main() {
  int width = 512;
  int height = 512;
  const uint8_t *data = (const uint8_t *)image_data;

  // Armazena a imagem resultante
  uint8_t result[512 * 512];

  printf("###START###\n");
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      float sbl_x = 0;
      float sbl_y = 0;

      for (int ky = 0; ky < 3; ++ky) {
        for (int kx = 0; kx < 3; ++kx) {
          int offset_x = kx - 1;
          int offset_y = ky - 1;
          int curr_x = x + offset_x;
          int curr_y = y + offset_y;
          int kernel_index = ky * 3 + kx;
          int data_index = curr_y * width + curr_x;

          sbl_x += sobel_x[kernel_index] * data[data_index];
          sbl_y += sobel_y[kernel_index] * data[data_index];
        }
      }

      float mag = sqrtf(sbl_x * sbl_x + sbl_y * sbl_y);
      uint8_t val = (mag > 255.0f) ? 255 : (uint8_t)mag;

      result[y * width + x] = val;
    }
  }
  printf("###END###\n");

  // Impressão da imagem resultante (fora da contagem de instruções)
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      printf("%3d ", result[y * width + x]);
    }
    printf("\n");
  }

  return 0;
}

