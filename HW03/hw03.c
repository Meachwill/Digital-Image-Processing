#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "./lib/bmp_point.h"
#include <math.h>
#define image_name "CUB.bmp"
//imagetest=========================
#define image_namelenal "lena512.bmp"
#define image_test "testgrayred.bmp"
//=================================
#define image_output "balltest.bmp"
#define image_outputR "balltest_R.bmp"
#define image_outputG "balltest_G.bmp"
#define image_outputB "balltest_B.bmp"

double mask_sharpen[3][3] = {{1.0, 1.0, 1.0},
                             {1.0, -8.0, 1.0},
                             {1.0, 1.0, 1.0}};
// double mask_smooth[3][3] = {{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
//                             {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
//                             {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}};
double mask_smooth[3][3] = {{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
                            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
                            {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}};
double mask_morphological1[3][3] = {{1.0, 1.0, 1.0},
                                    {1.0, 1.0, 1.0},
                                    {1.0, 1.0, 1.0}};
double mask_morphological2[3][3] = {{0, 1.0, 0},
                                    {1.0, 1.0, 1.0},
                                    {0, 1.0, 0}};
// double mask_morphological3[5][5] = {{1.0, 1.0, 1.0, 1.0},
//                                     {1.0, 1.0, 1.0, 1.0},
//                                     {1.0, 1.0, 1.0, 1.0},
//                                     {1.0, 1.0, 1.0, 1.0},
//                                     {1.0, 1.0, 1.0, 1.0}};
// double mask_morphological3[5][5] = {{0, 0, 1.0, 0, 0},
//                                     {0, 1.0, 1.0, 0, 0},
//                                     {1.0, 1.0, 1.0, 1.0, 1.0},
//                                     {0, 1.0, 1.0, 0, 0},
//                                     {0, 0, 1.0, 0, 0}};
double mask_morphological3[9][9] = {{0, 0, 0, 0, 1.0, 0, 0, 0, 0},
                                    {0, 0, 0, 1.0, 1.0, 1.0, 0, 0, 0},
                                    {0, 0, 1.0, 1.0, 1.0, 1.0, 1.0, 0, 0},
                                    {0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0},
                                    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                                    {0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0},
                                    {0, 0, 1.0, 1.0, 1.0, 1.0, 1.0, 0, 0},
                                    {0, 0, 0, 1.0, 1.0, 1.0, 0, 0, 0},
                                    {0, 0, 0, 0, 1.0, 0, 0, 0, 0}};
uint8_t mask_size = (sizeof(mask_sharpen) / sizeof(mask_sharpen[0][0]));
uint8_t mask_morphological_size = (sizeof(mask_morphological1) / sizeof(mask_morphological1[0][0]));
uint8_t **BMP_READ24bit(char *filename, BMP *bmp);
void BMP_WRITE8bit(char *filename, BMP *bmp, uint8_t *image, uint8_t level);
void BMP_WRITE24bit(char *filename, BMP *bmp, uint8_t **img, uint32_t size);
void watershed(uint8_t *img, BMP *bmp);
void weightchaeng(uint8_t *img, uint32_t img_size, uint8_t level, uint8_t new_level);

    int main()
{
    BMP bmp;
    BMP bmp_lana;
    double *img_convolution = (double *)malloc(sizeof(double) * bmp.bitmap_data_size);
    double *img_double = (double *)malloc(sizeof(double) * bmp.bitmap_data_size);
    uint8_t *img_new = (uint8_t *)malloc(sizeof(uint8_t) * bmp.bitmap_data_size);
    printf("test01\n");
    uint8_t **img1 = BMP_READ24bit(image_name, &bmp);         //讀24bit
    uint8_t *img2 = BMP_READ8bit(image_namelenal, &bmp_lana); //讀8bit
    uint8_t *blue = img1[0];
    uint8_t *green = img1[1];
    uint8_t *red = img1[2];
    // FILE *image_org = image_org = fopen(image_name, "rb");
    // fread(bmp.header, sizeof(uint8_t), header_size, image_org);
    // fread(bmp.info, sizeof(uint8_t), info_size, image_org);
    // BMP_Data2dec(&bmp);
    BMP_Print(&bmp);
    BMP_Data_Reset(&bmp, set_size, (512 * 512) + 1024 + 54);
    BMP_Data_Reset(&bmp, set_bitmap_data_offset, 1024 + 54);
    BMP_Data_Reset(&bmp, set_bitmap_data_size, 512 * 512);
    BMP_Data_Reset(&bmp, set_bits_per_pixel, 8);
    BMP_Data_Reset(&bmp, set_used_colors, 256);
    img_double = uint8_double(red, bmp.bitmap_data_size);
    img_convolution = convolution(img_double, &mask_sharpen[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    gray_thresholding(img_convolution, bmp.bitmap_data_size, 30);
    img_convolution = convolution(img_convolution, &mask_smooth[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    img_convolution = convolution(img_convolution, &mask_smooth[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    img_convolution = convolution(img_convolution, &mask_smooth[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    img_convolution = convolution(img_convolution, &mask_smooth[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    img_convolution = convolution(img_convolution, &mask_smooth[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    img_convolution = dilation(img_convolution, &mask_morphological1[0][0], bmp.bitmap_data_size, mask_morphological_size, bmp.width, bmp.height);
    img_convolution = erosion(img_convolution, &mask_morphological1[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    img_convolution = erosion(img_convolution, &mask_morphological3[0][0], bmp.bitmap_data_size, mask_size, bmp.width, bmp.height);
    
    img_new = double_uint8(img_convolution, bmp.bitmap_data_size);
    watershed(img_new, &bmp);
    BMP_WRITE8bit(image_test, &bmp, img_new, 0);
    // BMP_WRITE24bit(image_output, &bmp, img, bmp.bitmap_data_size);
    //================================test01===================================
    // BMP_Print(&bmp_lana);
    // img2 = downsample(&bmp_lana, img2, bmp_lana.bitmap_data_size, bmp_lana.width, bmp_lana.height);
    // img_double = uint8_double(img2, bmp_lana.bitmap_data_size);
    // img_convolution = convolution(img_double, &mask_sharpen[0][0], bmp_lana.bitmap_data_size, mask_size, bmp_lana.width, bmp_lana.height);
    // gray_thresholding(img_convolution, bmp_lana.bitmap_data_size, 50);
    // img_convolution = dilation(img_convolution, &mask_morphological[0][0], bmp_lana.bitmap_data_size, mask_morphological_size, bmp_lana.width, bmp_lana.height);
    // img_convolution = erosion(img_convolution, &mask_morphological[0][0], bmp_lana.bitmap_data_size, mask_size, bmp_lana.width, bmp_lana.height);
    // img_new = double_uint8(img_convolution, bmp_lana.bitmap_data_size);
    // BMP_WRITE8bit(image_test, &bmp_lana, img2, gray_8bit);
    //=========================================================================
}
// BMP_Data_Reset(&temp_bmp, set_size, (512 * 512) + 1024 + 54);
// BMP_Data_Reset(&temp_bmp, set_bitmap_data_offset, 1024 + 54);
// BMP_Data_Reset(&temp_bmp, set_bitmap_data_size, 512 * 512);
// BMP_Data_Reset(&temp_bmp, set_bits_per_pixel, 8);
// BMP_Data_Reset(&temp_bmp, set_used_colors, 256);
uint8_t **BMP_READ24bit(char *filename, BMP *bmp)
{
    FILE *image_org = image_org = fopen(filename, "rb");
    fread(bmp->header, sizeof(uint8_t), header_size, image_org);
    fread(bmp->info, sizeof(uint8_t), info_size, image_org);
    BMP_Data2dec(bmp);
    uint8_t *B = (uint8_t *)malloc((bmp->width) * (bmp->height));
    uint8_t *G = (uint8_t *)malloc((bmp->width) * (bmp->height));
    uint8_t *R = (uint8_t *)malloc((bmp->width) * (bmp->height));
    int image_pix = bmp->bitmap_data_size;
    uint8_t *img = (uint8_t *)malloc(image_pix * (sizeof(uint8_t)));
    fread(img, sizeof(uint8_t), image_pix, image_org);

    for (uint16_t x = 0; x < bmp->width; x++)
    {
        for (uint16_t y = 0; y < bmp->height; y++)
        {
            B[(x * bmp->width) + y] = img[3 * ((x * (bmp->width)) + y) + 0];
            G[(x * bmp->width) + y] = img[3 * ((x * (bmp->width)) + y) + 1];
            R[(x * bmp->width) + y] = img[3 * ((x * (bmp->width)) + y) + 2];
        }
    }
    uint8_t **img_BGR = (uint8_t **)malloc(3);
    img_BGR[0] = B;
    img_BGR[1] = G;
    img_BGR[2] = R;
    fclose(image_org);
    printf("read ok\n");
    return img_BGR;
}

void BMP_WRITE24bit(char *filename, BMP *bmp, uint8_t **img, uint32_t size)
{
    uint32_t bitmap_size = header_size + info_size + size;
    uint8_t *temp = (uint8_t *)malloc(sizeof(uint8_t) * size);
    struct ioutput
    {
        uint8_t header_output[header_size];
        uint8_t info_output[info_size];
        uint8_t image_out[size];
    } ioutput;
    uint8_t *b = img[0];
    uint8_t *g = img[1];
    uint8_t *r = img[2];
    printf("size=%d\n", bitmap_size);
    for (uint16_t x = 0; x < bmp->width; x++)
    {
        for (uint16_t y = 0; y < bmp->height; y++)
        {
            temp[3 * ((x * bmp->width) + y) + 0] = b[(x * bmp->width) + y];
            temp[3 * ((x * bmp->width) + y) + 1] = g[(x * bmp->width) + y];
            temp[3 * ((x * bmp->width) + y) + 2] = r[(x * bmp->width) + y];
        }
    }
    memcpy(ioutput.header_output, bmp->header, header_size);
    memcpy(ioutput.info_output, bmp->info, info_size);
    memcpy(ioutput.image_out, temp, size);
    FILE *NewImageFile = fopen(filename, "wb");
    fwrite(ioutput.header_output, 1, bitmap_size, NewImageFile);
    fclose(NewImageFile);
    printf("write ok\n");
}

void watershed(uint8_t *img, BMP *bmp)
{
    uint8_t edge = 1;
    uint8_t set=1  ;
    uint8_t point_color = 1;
    printf("img=%d\n", img[125 + 512]);
    for (uint16_t x = 1; x < bmp->width; x++)
    {
        for (uint16_t y = 1; y < bmp->height; y++)
        {
            if (img[y + (x * bmp->width)] > 254)
            {
                edge = 1;
            }
            else
            {
                if (edge == 1)
                {
                    if (img[y + ((x - 1) * bmp->width)] > 254)
                    {
                        point_color += 1;
                        img[y + (x * bmp->width)] = point_color;
                        if (point_color > 253)
                        {
                            set+=1;
                            point_color = 65+set;
                        }
                    }
                    else
                    {
                        img[y + (x * bmp->width)] = img[y + ((x - 1) * bmp->width)];
                    }
                    edge = 0;
                }
                else
                {
                    if (img[y + ((x - 1) * bmp->width)] > 254)
                    {
                        img[y + (x * bmp->width)] = img[y - 1 + (x * bmp->width)];
                    }
                    else
                    {
                        if (img[y - 1 + (x * bmp->width)] == img[y + ((x - 1) * bmp->width)])
                        {
                            img[y + (x * bmp->width)] = img[y + ((x-1)* bmp->width)];
                        }
                        else
                        {
                            img[y + (x * bmp->width)] = img[(y - 1) + (x * bmp->width)];
                            weightchaeng(img, bmp->width * bmp->height, img[y + ((x - 1) * bmp->width)], img[y + (x * bmp->width)]);

                        }
                    }
                }
            }
        }
    }

    // for (uint16_t y = 0; y < convolution_y; y++)
    // {
    //     for (uint16_t x = 0; x < convolution_x; x++)
    //     {

    //     }
    // }
}

void weightchaeng(uint8_t *img, uint32_t img_size, uint8_t level,uint8_t new_level)
{
    for (int i = 0; i < img_size; i++)
    {
        if (img[i] == level)
        {
            img[i] = new_level;
        }
    }
}
