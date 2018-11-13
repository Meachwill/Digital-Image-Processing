#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#define pi 3.14159265359
#define image_pix 262144
#define header_size 14
#define info_size 40
#define color_size 1024
#define image_size 263222
#define U16(x) ((unsigned short)(x))
#define U32(x) ((int)(x))
#define b2u16(data, offset) (U16(data[offset]) | U16(data[offset + 1]) << 8)
#define b2u32(data, offset) (U32(data[offset]) | U32(data[offset + 1]) << 8 | U32(data[offset + 2]) << 16 | U32(data[offset + 3]) << 24)

typedef struct BMP
{
    uint8_t header[header_size];
    uint8_t info[info_size];
    uint8_t color[color_size];
    //header
    uint16_t id;
    uint32_t size;
    uint32_t reserve;
    uint32_t bitmap_data_offset;
    //info
    uint32_t bitmap_header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t bitmap_data_size;
    uint32_t H;
    uint32_t V;
    uint32_t used_colors;
    uint32_t important_colors;

} BMP;

typedef struct image_output_struct
{
    uint8_t header_TEST[header_size];
    uint8_t info_TEST[info_size];
    uint8_t color_TEST[color_size];
    uint8_t image_cod[image_pix];
} image_new_struct;

void BMP_DCT(double *matrix, double *oupt_matrix, uint8_t *oupt_dct);

double image_dct[512][512];
double image_new[512][512]={0};
double dct_01[64][64] = {0};
uint8_t image[512][512];
uint8_t quantization_matrix[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                                     {12, 12, 14, 19, 26, 58, 60, 55},
                                     {14, 13, 16, 24, 40, 57, 69, 56},
                                     {14, 17, 22, 29, 51, 87, 80, 62},
                                     {18, 22, 37, 56, 68, 109, 103, 77},
                                     {24, 35, 55, 64, 81, 104, 113, 92},
                                     {49, 64, 78, 87, 103, 121, 120, 101},
                                     {72, 92, 95, 98, 112, 100, 103, 99}};

int main()
{
    //test========================
    // uint8_t test_matrix[8][8]={{52,55,61,66,70,61,64,73},
    //                     {63,59,66,90,109,85,69,72},
    //                     {62,59,68,113,144,104,66,73},
    //                     {63,58,71,122,154,106,70,69},
    //                     {67,61,68,104,126,88,68,70},
    //                     {79,65,60,70,77,68,58,75},
    //                     {85,71,64,59,55,61,65,83},
    //                     {87,79,69,68,65,76,78,94}};
    // double test_matrix02[8][8]={0};
    // char test_dct[8][8] = {0};
    // char pic=0;
    //============================


    BMP bmp;
    FILE *image_org, *image_new_output, *image_dct_txt, *image_n128_txt, *image_out_test;
    image_new_struct IMAGE_NEW;
    BMP bmp_new;
    uint8_t image_new_out[512][512] = {0};
    char *abc = "lena512.bmp";

    //image in=====================================================
    image_org = fopen(abc, "rb");

    //fread binary data============================================
    fread(bmp.header, 1, header_size, image_org);
    fread(bmp.info, 1, info_size, image_org);
    fread(bmp.color, 1, color_size, image_org);
    fread(image, 1, image_pix, image_org);
    fclose(image_org);
    //==============================================================

    //=====================image_test===============================
    memcpy(IMAGE_NEW.header_TEST, bmp.header, header_size);
    memcpy(IMAGE_NEW.info_TEST, bmp.info, info_size);
    memcpy(IMAGE_NEW.color_TEST, bmp.color, color_size);
    memcpy(IMAGE_NEW.image_cod, image, image_pix);
    image_new_output = fopen("hw02_returntest.bmp", "wb");
    fwrite(IMAGE_NEW.header_TEST, 1, image_size, image_new_output);
    fclose(image_new_output);
    //shift-128=====================================================
    for (int i = 0; i < 512; i++)
    {
        for (int k = 0; k < 512; k++)
        {
            image_new[i][k] = ((double)image[i][k] - 128);
        }
    }
    image_n128_txt = fopen("test_128.txt", "w+t");
    for (int i = 0; i < 512; i++)
    {
        for (int k = 0; k < 512; k++)
        {
            fprintf(image_n128_txt, "%f\t", image_new[i][k]);
        }
    }
    fclose(image_n128_txt);
    //===============================================================

    //test_shift-128=================================================
    // for (int i = 0; i < 8; i++){
    //     for (int j = 0; j < 8; j++){
    //                  test_matrix02[i][j] = (double)(test_matrix[i][j] - 128);
    //                  printf("%.2f\t", test_matrix02[i][j]);
    //     }
    // }
    // BMP_DCT(&test_matrix02[0][0],&test_dct[0][0]);

    // image_dct_txt = fopen("test_dct_fuck.txt", "w+t");
    // for (int i = 0; i < 8; i++)
    // {
    //     for (int k = 0; k < 8; k++)
    //     {
    //         fprintf(image_dct_txt, "%d\t", test_dct[i][k]);
    //     }
    //     fprintf(image_dct_txt, "\n");
    // }
    // fclose(image_dct_txt);
    //==============================================================

    //DCT===========================================================
    for(int u=0;u<512;u+=8){
        for(int v=0;v<512;v+=8){
            BMP_DCT(&image_new[u][v], &image_dct[u][v], &image_new_out[u][v]);
        }
    }

    image_dct_txt = fopen("test_dct.txt", "w+t");
    for (int i = 0; i < 512; i++)
    {
        for (int k = 0; k < 512; k++){
            fprintf(image_dct_txt, "%f\t", image_dct[i][k]);
        }
        fprintf(image_dct_txt, "\n");
    }
    fclose(image_dct_txt);
    //==========================================================

    //new image output================================================================
    memcpy(IMAGE_NEW.header_TEST, bmp.header, header_size);
    memcpy(IMAGE_NEW.info_TEST, bmp.info, info_size);
    memcpy(IMAGE_NEW.color_TEST, bmp.color, color_size);
    memcpy(IMAGE_NEW.image_cod, image_new_out, image_pix);
    image_new_output = fopen("hw02_dct.bmp", "wb");
    fwrite(IMAGE_NEW.header_TEST, 1, image_size, image_new_output);
    fclose(image_new_output);
    // image_out_test = fopen("image_out_test.txt", "w+t");
    // for (int i = 0; i < 512; i++) 
    // {
    //     for (int k = 0; k < 512; k++)
    //     {
    //         fprintf(image_out_test, "%d\t", image_new_out[i][k]);
    //     }
    // }
    // fclose(image_out_test);
    // printf("%d",sizeof(double));
    return 0;
}

void BMP_DCT(double *matrix, double *oupt_matrix,uint8_t *oupt_dct)
{
    float c_u=0,c_v=0;
    double sum,dct1;
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
                    dct1 = (matrix[512*x+y])*
                           cos((double)(((2 * x + 1) * u * pi) / 16.0))*
                           cos((double)(((2 * y + 1) * v * pi) / 16.0));
                    sum = sum + dct1;
                }
            }
            *(oupt_matrix+(512*u)+v) =round(c_u * c_v * sum * 0.25);
            *(oupt_dct + (512*u) + v) = ((uint8_t)((*(oupt_matrix + (512* u) + v))/quantization_matrix[u][v]));
        }
    }
}