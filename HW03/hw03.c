#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "./lib/bmp_point.h"
#include <math.h>
#define image_name "lena512.bmp"
#define image_output "LENACON.bmp"

double *convolution(uint8_t *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width);

// double mask[3][3] = {{0, 1, 0},
//                      {1, -4, 1},
//                      {0, 1, 0}};
double mask[3][3] = {{1/16, 2/16, 1/16},
                     {2/16, 4/16, 2/16},
                     {1/16, 2/16, 1/16}};
uint8_t mask_size = (sizeof(mask) / sizeof(mask[0][0]));

int main()
{
    BMP bmp;
    double *img_convolution = (double *)malloc(sizeof(double) * bmp.bitmap_data_size);
    uint8_t *img_new = (uint8_t *)malloc(sizeof(uint8_t) * bmp.bitmap_data_size);
    uint8_t *img = BMP_READ(image_name, &bmp);
    BMP_print(&bmp);
    printf("mask=%d\n",(uint8_t)sqrt((double)mask_size));
    img_convolution = convolution(img, &mask[0][0], bmp.bitmap_data_size, mask_size, bmp.width);
    img_convolution=normalization(img_convolution, bmp.bitmap_data_size);
    img_new = double_uint8(img_convolution, bmp.bitmap_data_size);
    BMP_WRITE512(image_output, &bmp, img_new);
}

double *convolution(uint8_t *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width)
{
    double *img_convolution = (double *)malloc(sizeof(double) * img_size);
    uint32_t convolution_x = 512 - (uint8_t)sqrt((double)con_mask_size);
    uint32_t convolution_y = 512 - (uint8_t)sqrt((double)con_mask_size);
    for (uint32_t y = 0; y < convolution_y; y++)
    {
        for (uint32_t x = 0; x < convolution_x; x++)
        {
            *(img_convolution + y * width + x + width + 1) = (*(img + y * width + x)) * (*(con_mask + 0)) +
                                                             (*(img + y * width + x + 1)) * (*(con_mask + 1)) +
                                                             (*(img + y * width + x + 2)) * (*(con_mask + 2)) +
                                                             (*(img + y * width + x + width)) * (*(con_mask + 3)) +
                                                             (*(img + y * width + x + width + 1)) * (*(con_mask + 4)) +
                                                             (*(img + y * width + x + width + 2)) * (*(con_mask + 5)) +
                                                             (*(img + y * width + x + 2 * width)) * (*(con_mask + 6)) +
                                                             (*(img + y * width + x + 2 * width + 1)) * (*(con_mask + 7)) +
                                                             (*(img + y * width + x + 2 * width + 2)) * (*(con_mask + 8));
        }
    }
    return img_convolution;
}

// uint8_t graylevel256(BMP *img){

// }
