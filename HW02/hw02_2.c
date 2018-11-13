#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include "lib/bmp_point.h"
#define image_name "lena512.bmp"
#define image_down_name "lena256_downsample.bmp"
#define image_down_dct_name "lena256_down_dct.bmp"
#define image_down_idct_name "lena256_down_idct.bmp"
#define image_down_sub_name "lena256_down_sub.bmp"
#define pi 3.14159265359
#define image_axis_x 256
#define image_axis_y 256
#define image_pixel image_axis_x *image_axis_y
#define image_size image_pixel + header_size + info_size + color_size

double *BMP_DCT_transmit(uint8_t *org_image, int size);
void BMP_DCT(uint8_t *matrix, double *oupt_matrix, uint16_t start_x, uint16_t start_y);
void Inverse_Quantization(double *image_dct);
double *BMP_IDCT_transmit(double *dct_image, int size);
void BMP_IDCT(double *dct_image, double *oupt_matrix, uint16_t start_x, uint16_t start_y);
uint8_t *double_uint8(double *image, uint32_t size);
double MSE(uint8_t *image, uint8_t *new_image);
double PSNR(double mse);
double *image_sub(uint8_t *image, double *image_new, int size);
double image_new[image_axis_x][image_axis_y];
uint8_t *downsample(uint8_t *image,int size);
double *normalization(double *image, int size);

uint8_t quantization_matrix[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                                     {12, 12, 14, 19, 26, 58, 60, 55},
                                     {14, 13, 16, 24, 40, 57, 69, 56},
                                     {14, 17, 22, 29, 51, 87, 80, 62},
                                     {18, 22, 37, 56, 68, 109, 103, 77},
                                     {24, 35, 55, 64, 81, 104, 113, 92},
                                     {49, 64, 78, 87, 103, 121, 120, 101},
                                     {72, 92, 95, 98, 112, 100, 103, 99}};

uint8_t FilterMask[8][8] = {
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

int main()
{
    BMP bmp;
    FILE *image_dct_txt;
    double mse = 0;
    printf("//===========================main2=====================================//\n");
    //=======================input image===============================
    uint8_t *image = BMP_READ(image_name, &bmp);
    //=======================downsampling==============================
    uint8_t *image_downsample=downsample(image,image_pixel);
    bmp.width=256;
    bmp.height=256;
    bmp.size=256*256+54+1024;
    bmp.bitmap_data_size=256*256;
    bmp.header[2] = (0xff & ((bmp.size) >> 0));
    bmp.header[3] = (0xff & ((bmp.size) >> 8));
    bmp.header[4] = (0xff & ((bmp.size) >> 16));
    bmp.header[5] = (0xff & ((bmp.size) >> 24));
    bmp.info[4] = (0xff & ((bmp.width) >> 0));
    bmp.info[5] = (0xff & ((bmp.width) >> 8));
    bmp.info[6] = (0xff & ((bmp.width) >> 16));
    bmp.info[7] = (0xff & ((bmp.width) >> 24));
    bmp.info[8] = (0xff & ((bmp.height) >> 0));
    bmp.info[9] = (0xff & ((bmp.height) >> 8));
    bmp.info[10] = (0xff & ((bmp.height) >> 16));
    bmp.info[11] = (0xff & ((bmp.height) >> 24));
    bmp.info[20] = (0xff & ((bmp.bitmap_data_size) >> 0));
    bmp.info[21] = (0xff & ((bmp.bitmap_data_size) >> 8));
    bmp.info[22] = (0xff & ((bmp.bitmap_data_size) >> 16));
    bmp.info[23] = (0xff & ((bmp.bitmap_data_size) >> 24));
    //===================downsampling output===========================
    BMP_WRITE256(image_down_name, &bmp, image_downsample);
    //===========================DCT===================================
    double *image_dct = BMP_DCT_transmit(image_downsample, image_pixel);
    // //=====================DCT image output============================
    BMP_WRITE256(image_down_dct_name, &bmp, double_uint8(image_dct, image_pixel));
    // // //===========================IQ====================================
    // Inverse_Quantization(image_dct);
    // // //===========================IDCT===============================
    double *image_idct = BMP_IDCT_transmit(image_dct, image_pixel);
    // // //========================iDCT image output=====================
    uint8_t *new_image = double_uint8(image_idct, image_pixel);
    BMP_WRITE256(image_down_idct_name, &bmp, new_image);
    // // //==============================================================
    double *image_subtraction_u32 = image_sub(image_downsample, image_idct, image_pixel);
    double *nor_image = normalization(image_subtraction_u32, image_pixel);
    uint8_t *image_subtraction_u8 = double_uint8(nor_image, image_pixel);
    BMP_WRITE256(image_down_sub_name, &bmp, image_subtraction_u8);
    // // //==============================================================
    printf("//==========================analyze====================================//\n");
    mse = MSE(image_downsample, new_image);
    PSNR(mse);
    
    system("pause");
    return 0;
}

void BMP_DCT(uint8_t *matrix, double *oupt_matrix, uint16_t start_x, uint16_t start_y)
{
    float c_u = 0, c_v = 0;
    double sum, temp, dct1;
    //===============================test=================================
    for (int u = 0; u < 8; u++)
    {
        for (int v = 0; v < 8; v++)
        {
            if (u == 0)
            {
                c_u = 1 / sqrt(2);
            }
            else
            {
                c_u = 1;
            }
            if (v == 0)
            {
                c_v = 1 / sqrt(2);
            }
            else
            {
                c_v = 1;
            }
            sum = 0;
            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    temp = ((double)(matrix[(start_x + x) * image_axis_x + y + start_y])) - 128;
                    dct1 = (double)((temp)*cos((double)((2 * x + 1) * (u)*pi) / 16.0) *
                                    cos((double)(((2 * (y) + 1) * v * pi)) / 16.0));
                    sum = sum + dct1;
                }
            }
            oupt_matrix[(u + start_x) * image_axis_x + v + start_y] = round(((c_u * c_v * sum * 0.25) * FilterMask[u][v]));
        }
    }
}

double *BMP_DCT_transmit(uint8_t *org_image, int size)
{
    double *new_img = (double *)malloc(size * sizeof(double));
    for (int u = 0; u < image_axis_x; u += 8)
    {
        for (int v = 0; v < image_axis_y; v += 8)
        {
            BMP_DCT(org_image, new_img, u, v);
        }
    }
    return new_img;
}

void Inverse_Quantization(double *dct_image)
{
    for (int x = 0; x < image_axis_x; x += 8)
    {
        for (int y = 0; y < image_axis_y; y += 8)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    dct_image[(x + i) * image_axis_x + y + j] *= quantization_matrix[i][j];
                }
            }
        }
    }
}

double *BMP_IDCT_transmit(double *dct_image, int size)
{
    double *idct_img = (double *)malloc(size * sizeof(double));
    for (int u = 0; u < image_axis_x; u += 8)
    {
        for (int v = 0; v < image_axis_y; v += 8)
        {
            BMP_IDCT(dct_image, idct_img, u, v);
        }
    }
    return idct_img;
}

void BMP_IDCT(double *dct_image, double *oupt_matrix, uint16_t start_x, uint16_t start_y)
{
    float c_u = 0, c_v = 0;
    double sum, temp, dct1;
    //===============================test=================================
    for (int u = 0; u < 8; u++)
    {
        for (int v = 0; v < 8; v++)
        {

            sum = 0;
            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    if (x == 0)
                    {
                        c_u = 1 / sqrt(2);
                    }
                    else
                    {
                        c_u = 1;
                    }
                    if (y == 0)
                    {
                        c_v = 1 / sqrt(2);
                    }
                    else
                    {
                        c_v = 1;
                    }
                    dct1 = (double)(c_u * c_v * (dct_image[(x + start_x) * image_axis_x + y + start_y]) * cos((double)((2.0 * u + 1) * x * pi / 16.0)) *
                                    cos((double)((2.0 * v + 1) * y * pi / 16.0)));
                    sum = sum + dct1;
                }
            }
            oupt_matrix[(u + start_x) * image_axis_x + v + start_y] = (sum * 0.25) + 128;
        }
    }
}

uint8_t *double_uint8(double *image, uint32_t size)
{
    uint8_t *uint_image = (uint8_t *)(malloc(size));
    for (uint32_t i = 0; i < size; i++)
    {
        uint_image[i] = (uint8_t)round(image[i]);
    }
    return uint_image;
}

double MSE(uint8_t *image, uint8_t *new_image)
{
    int temp = 0, sum = 0;
    double mse = 0;
    for (int x = 0; x < image_pixel; x++)
    {
        temp = (*(image + x)) - (*(new_image + x));
        sum += (temp * temp);
    }
    mse = (double)(sum) / (double)(image_pixel);
    printf("MSE=%f\n", mse);
    return (mse);
}

double PSNR(double mse)
{
    double temp = 0;
    temp = 10 * log((pow(255, 2) / mse));
    printf("PSNR=%f\n", temp);
    return (temp);
}

double *image_sub(uint8_t *image, double *image_new, int size)
{
    double *image_output = (double *)malloc(size * sizeof(double));
    for (int x = 0; x < image_pixel; x++)
    {
        *(image_output + x) = abs((int)(*(image_new + x) - ((double)(*(image + x)))));
    }
    return (image_output);
}

uint8_t *downsample(uint8_t *image,int size){
    uint8_t *image_output = (uint8_t *)malloc(size * sizeof(uint8_t));
    int tmp=0;
    for (int x = 0; x < 512; x++){
        for (int y = 0; y < 512; y++){
            if(x%2==0 && y%2==0){
            // if ((x & 1) && (y & 1)){
                image_output[tmp] = image[x*512 + y];
                tmp++;
            }
        }
    }
    return(image_output);
}
double *normalization(double *image, int size)
{
    double *normaliz_image = (double *)malloc(size * sizeof(double));
    double max = *image, min = *image;
    for (int i = 0; i < size; i++)
    {
        if ((*(image + i)) > max)
        {
            max = *(image + i);
        }
        if ((*(image + i)) < min)
        {
            min = *(image + i);
        }
    }
    for (int x = 0; x < size; x++)
    {
        (*(normaliz_image + x)) = (((*(image + x)) - min) / (max - min)) * 255.0;
    }
    return normaliz_image;
}