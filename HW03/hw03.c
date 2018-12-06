#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "./lib/bmp_point.h"
#include <math.h>
#define image_name "lena512.bmp"
#define image_output "LENACON.bmp"

double *convolution(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width);
void graylevel(double *img, uint32_t img_size, uint8_t level);
double *dilation(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width);
double *erosion(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width);
double mask_sharpen[3][3] = {{0, 1.0, 0},
                             {1.0, -4.0, 1.0},
                             {0.0, 1.0, 0}};
// double mask_smooth[3][3] = {{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
//                             {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
//                             {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}};
double mask_smooth[3][3] = {{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
                            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
                            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}};
// double mask_morphological[3][3] = {{1.0, 1.0, 1.0},
//                                    {1.0, 1.0, 1.0},
//                                    {1.0, 1.0, 1.0}};
double mask_morphological[3][3] = {{0, 1.0, 0},
                                   {1.0, 1.0, 1.0},
                                   {0, 1.0, 0}};
// double mask_morphological[4][4] = {{0, 1.0, 1.0, 0},
//                                    {1.0, 1.0, 1.0, 1.0},
//                                    {0, 1.0, 1.0, 0}};
uint8_t mask_size = (sizeof(mask_sharpen) / sizeof(mask_sharpen[0][0]));
uint8_t mask_morphological_size = (sizeof(mask_morphological) / sizeof(mask_morphological[0][0]));

int main()
{
    BMP bmp;
    double *img_convolution = (double *)malloc(sizeof(double) * bmp.bitmap_data_size);
    double *img_double = (double *)malloc(sizeof(double) * bmp.bitmap_data_size);
    uint8_t *img_new = (uint8_t *)malloc(sizeof(uint8_t) * bmp.bitmap_data_size);
    uint8_t *img = BMP_READ(image_name, &bmp);
    BMP_print(&bmp);
    img_double = uint8_double(img, bmp.bitmap_data_size);
    img_convolution = convolution(img_double, &mask_sharpen[0][0], bmp.bitmap_data_size, mask_size, bmp.width);
    graylevel(img_convolution, bmp.bitmap_data_size, 50);
    img_convolution = dilation(img_convolution, &mask_morphological[0][0], bmp.bitmap_data_size, mask_morphological_size, bmp.width);
    img_convolution = erosion(img_convolution, &mask_morphological[0][0], bmp.bitmap_data_size, mask_size, bmp.width);
    img_new = double_uint8(img_convolution, bmp.bitmap_data_size);
    BMP_WRITE512(image_output, &bmp, img_new);
}

double *convolution(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width)
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

void graylevel(double *img, uint32_t img_size, uint8_t level)
{
    for (int i = 0; i < img_size; i++)
    {
        if (img[i] > level)
        {
            img[i] = 255;
        }
        else
        {
            img[i] = 0;
        }
    }
}
double *dilation(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width)
{
    double *img_convolution = (double *)malloc(sizeof(double) * img_size);
    uint32_t convolution_x = 512 - (uint8_t)sqrt((double)con_mask_size);
    uint32_t convolution_y = 512 - (uint8_t)sqrt((double)con_mask_size);
    double sum = 0;
    for (uint32_t y = 0; y < convolution_y; y++)
    {
        for (uint32_t x = 0; x < convolution_x; x++)
        {
            sum = (*(img + y * width + x)) * (*(con_mask + 0)) +
                  (*(img + y * width + x + 1)) * (*(con_mask + 1)) +
                  (*(img + y * width + x + 2)) * (*(con_mask + 2)) +
                  (*(img + y * width + x + width)) * (*(con_mask + 3)) +
                  (*(img + y * width + x + width + 1)) * (*(con_mask + 4)) +
                  (*(img + y * width + x + width + 2)) * (*(con_mask + 5)) +
                  (*(img + y * width + x + 2 * width)) * (*(con_mask + 6)) +
                  (*(img + y * width + x + 2 * width + 1)) * (*(con_mask + 7)) +
                  (*(img + y * width + x + 2 * width + 2)) * (*(con_mask + 8));
            if (sum >= 255)
            {
                *(img_convolution + y * width + x + width + 1) = 255;
            }
            else
            {
                *(img_convolution + y * width + x + width + 1) = 0;
            }
        }
    }
    return img_convolution;
}

double *erosion(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width)
{
    double *img_convolution = (double *)malloc(sizeof(double) * img_size);
    uint32_t convolution_x = 512 - (uint8_t)sqrt((double)con_mask_size);
    uint32_t convolution_y = 512 - (uint8_t)sqrt((double)con_mask_size);
    double sum = 0;
    uint32_t mask_point = 0;
    for (uint32_t k = 0; k < con_mask_size; k++)
    {
        if (*(con_mask + k) == 1)
        {
            mask_point++;
        }
    }
    for (uint32_t y = 0; y < convolution_y; y++)
    {
        for (uint32_t x = 0; x < convolution_x; x++)
        {
            sum = (*(img + y * width + x)) * (*(con_mask + 0)) +
                  (*(img + y * width + x + 1)) * (*(con_mask + 1)) +
                  (*(img + y * width + x + 2)) * (*(con_mask + 2)) +
                  (*(img + y * width + x + width)) * (*(con_mask + 3)) +
                  (*(img + y * width + x + width + 1)) * (*(con_mask + 4)) +
                  (*(img + y * width + x + width + 2)) * (*(con_mask + 5)) +
                  (*(img + y * width + x + 2 * width)) * (*(con_mask + 6)) +
                  (*(img + y * width + x + 2 * width + 1)) * (*(con_mask + 7)) +
                  (*(img + y * width + x + 2 * width + 2)) * (*(con_mask + 8));
            if (sum > (255 * (mask_point - 1)))
            {
                *(img_convolution + y * width + x + width + 1) = 255;
            }
            else
            {
                *(img_convolution + y * width + x + width + 1) = 0;
            }
        }
    }
    return img_convolution;
}
