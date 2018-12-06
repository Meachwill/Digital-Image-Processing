#ifndef _BMP_POINT_H_
#define _BMP_POINT_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#define header_size 14
#define info_size 40
#define color_size 1024
//======================= Data Calculate======================
#define U16(x) ((unsigned short)(x))
#define U32(x) ((int)(x))
#define b2u16(data, offset) (U16(data[offset]) | U16(data[offset + 1]) << 8)
#define b2u32(data, offset) (U32(data[offset]) | U32(data[offset + 1]) << 8 | U32(data[offset + 2]) << 16 | U32(data[offset + 3]) << 24)
//============================================================
#define set_id 0
#define set_size 1
#define set_reserve 2
#define set_bitmap_data_offset 3
#define set_bitmap_header_size 4
#define set_width 5
#define set_height 6
#define set_planes 7
#define set_bits_per_pixel 8
#define set_compression 9
#define set_bitmap_data_size 10
#define set_H 11
#define set_V 12
#define set_used_colors 13
#define set_important_colors 14

#pragma pack(1)
typedef struct __attribute__((packed)) BMP
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

typedef struct __attribute__((packed)) IMAGE512_OUTPUT_STRUCT
{
    uint8_t header_new[header_size];
    uint8_t info_new[info_size];
    uint8_t color_new[color_size];
    uint8_t image_new[512 * 512];
} image512_output_struct;
typedef struct __attribute__((packed)) IMAGE256_OUTPUT_STRUCT
{
    uint8_t header_new[header_size];
    uint8_t info_new[info_size];
    uint8_t color_new[color_size];
    uint8_t image_new[256 * 256];
} image256_output_struct;

uint8_t *BMP_READ(char *filename, BMP *bmp);
void BMP_data2dec(BMP *bmp);
void BMP_WRITE512(char *filename, BMP *bmp, uint8_t *image);
void BMP_WRITE256(char *filename, BMP *bmp, uint8_t *image);
void BMP_data_reset(BMP *bmp, char option, int data);
void BMP_print(BMP *bmp);
uint8_t *downsample(uint8_t *image, int size);
double MSE(uint8_t *image, uint8_t *new_image, uint32_t image_pix);
double PSNR(double mse);
double *normalization(double *image, uint32_t image_pix);
double *image_sub(uint8_t *image, double *image_new, uint32_t size);
uint8_t *double_uint8(double *image, uint32_t size);
double *uint8_double(uint8_t *image, uint32_t image_pix);
#endif