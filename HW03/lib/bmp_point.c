#include "bmp_point.h"

uint8_t *BMP_READ8bit(char *filename, BMP *bmp)
{
    FILE *image_org = image_org = fopen(filename, "rb");
    fread(bmp->header, sizeof(uint8_t), header_size, image_org);
    fread(bmp->info, sizeof(uint8_t), info_size, image_org);
    fread(bmp->color, sizeof(uint8_t), color_size, image_org);
    BMP_Data2dec(bmp);
    int image_pix = (bmp->width) * (bmp->height);
    uint8_t *img = (uint8_t *)malloc(image_pix * (sizeof(uint8_t)));
    fread(img, sizeof(uint8_t), image_pix, image_org);
    fclose(image_org);
    printf("read ok\n");
    return img;
}
void BMP_WRITE8bit(char *filename, BMP *bmp, uint8_t *image, uint8_t level)
{
    BMP temp_bmp = *bmp;
    uint32_t bitmap_size = header_size + info_size + color_size + temp_bmp.bitmap_data_size;
    uint8_t *color_temp = (uint8_t *)malloc(sizeof(uint8_t) * color_size);
    struct ioutput
    {
        uint8_t header_output[header_size];
        uint8_t info_output[info_size];
        uint8_t color_output[color_size];
        uint8_t imageoutput[temp_bmp.bitmap_data_size];
    } ioutput;
    FILE *NewImageFile = fopen(filename, "wb");
    switch (level)
    {
    case 0:
        for (uint16_t x = 0; x < 256; x++)
        {
            color_temp[x * 4 + 0] = x;
            color_temp[x * 4 + 1] = x;
            color_temp[x * 4 + 2] = x;
            color_temp[x * 4 + 3] = x;
        }
        memcpy(ioutput.header_output, temp_bmp.header, header_size);
        memcpy(ioutput.info_output, temp_bmp.info, info_size);
        memcpy(ioutput.color_output, color_temp, color_size);
        memcpy(ioutput.imageoutput, image, temp_bmp.bitmap_data_size);
        fwrite(ioutput.header_output, 1, bitmap_size, NewImageFile);
        fclose(NewImageFile);
        printf("write ok\n");
        break;
    case 1:
        memcpy(ioutput.header_output, temp_bmp.header, header_size);
        memcpy(ioutput.info_output, temp_bmp.info, info_size);
        memcpy(ioutput.color_output, temp_bmp.color, color_size);
        memcpy(ioutput.imageoutput, image, temp_bmp.bitmap_data_size);
        fwrite(ioutput.header_output, 1, bitmap_size, NewImageFile);
        fclose(NewImageFile);
        printf("write ok\n");
        break;
    case 2:
        for (uint16_t x = 0; x < 256; x++)
        {
            color_temp[x * 4 + 0] = x*5;
            color_temp[x * 4 + 1] = x*9;
            color_temp[x * 4 + 2] = x*17;
            color_temp[x * 4 + 3] = 0;
        }
        memcpy(ioutput.header_output, temp_bmp.header, header_size);
        memcpy(ioutput.info_output, temp_bmp.info, info_size);
        memcpy(ioutput.color_output, color_temp, color_size);
        memcpy(ioutput.imageoutput, image, temp_bmp.bitmap_data_size);
        fwrite(ioutput.header_output, 1, bitmap_size, NewImageFile);
        fclose(NewImageFile);
        printf("write ok\n");
        break;
    }
}

void BMP_Data2dec(BMP *bmp)
{
    bmp->id = b2u16(bmp->header, 0);
    bmp->size = b2u32(bmp->header, 2);
    bmp->reserve = b2u32(bmp->header, 6);
    bmp->bitmap_data_offset = b2u32(bmp->header, 10);
    bmp->bitmap_header_size = b2u32(bmp->info, 0);
    bmp->width = b2u32(bmp->info, 4);
    bmp->height = b2u32(bmp->info, 8);
    bmp->planes = b2u16(bmp->info, 12);
    bmp->bits_per_pixel = b2u16(bmp->info, 14);
    bmp->compression = b2u16(bmp->info, 16);
    bmp->bitmap_data_size = b2u32(bmp->info, 20);
    bmp->H = b2u32(bmp->info, 24);
    bmp->V = b2u32(bmp->info, 28);
    bmp->used_colors = b2u32(bmp->info, 32);
    bmp->important_colors = b2u32(bmp->info, 36);
}

void BMP_Print(BMP *bmp)
{
    printf("=================BMP HEADER DATA=================\n");
    printf("ID=%d\n", bmp->id);
    printf("size=%d\n", bmp->size);
    printf("reserve=%d\n", bmp->reserve);
    printf("bitmap_data_offset=%d\n", bmp->bitmap_data_offset);
    printf("bitmap_header_size=%d\n", bmp->bitmap_header_size);
    printf("width=%d\n", bmp->width);
    printf("height=%d\n", bmp->height);
    printf("planes=%d\n", bmp->planes);
    printf("bits_per_pixel=%d\n", bmp->bits_per_pixel);
    printf("compression=%d\n", bmp->compression);
    printf("bitmap_data_size=%d\n", bmp->bitmap_data_size);
    printf("H=%d\n", bmp->H);
    printf("V=%d\n", bmp->V);
    printf("used_colors=%d\n", bmp->used_colors);
    printf("important_colors=%d\n", bmp->important_colors);
    printf("=================================================\n");
}

void BMP_Data_Reset(BMP *bmp, char option, int data)
{
    switch (option)
    {
    case 0:
        bmp->id = data;
        bmp->header[0] = (0xff & ((bmp->id) >> 0));
        bmp->header[1] = (0xff & ((bmp->id) >> 8));
        break;
    case 1:
        bmp->size = data;
        bmp->header[2] = (0xff & ((bmp->size) >> 0));
        bmp->header[3] = (0xff & ((bmp->size) >> 8));
        bmp->header[4] = (0xff & ((bmp->size) >> 16));
        bmp->header[5] = (0xff & ((bmp->size) >> 24));
        break;
    case 2:
        bmp->reserve = data;
        bmp->header[6] = (0xff & ((bmp->reserve) >> 0));
        bmp->header[7] = (0xff & ((bmp->reserve) >> 8));
        bmp->header[8] = (0xff & ((bmp->reserve) >> 16));
        bmp->header[9] = (0xff & ((bmp->reserve) >> 24));
        break;
    case 3:
        bmp->bitmap_data_offset = data;
        bmp->header[10] = (0xff & ((bmp->bitmap_data_offset) >> 0));
        bmp->header[11] = (0xff & ((bmp->bitmap_data_offset) >> 8));
        bmp->header[12] = (0xff & ((bmp->bitmap_data_offset) >> 16));
        bmp->header[13] = (0xff & ((bmp->bitmap_data_offset) >> 24));
        break;
    case 4:
        bmp->bitmap_header_size = data;
        bmp->info[0] = (0xff & ((bmp->bitmap_header_size) >> 0));
        bmp->info[1] = (0xff & ((bmp->bitmap_header_size) >> 8));
        bmp->info[2] = (0xff & ((bmp->bitmap_header_size) >> 16));
        bmp->info[3] = (0xff & ((bmp->bitmap_header_size) >> 24));
        break;
    case 5:
        bmp->width = data;
        bmp->info[4] = (0xff & ((bmp->width) >> 0));
        bmp->info[5] = (0xff & ((bmp->width) >> 8));
        bmp->info[6] = (0xff & ((bmp->width) >> 16));
        bmp->info[7] = (0xff & ((bmp->width) >> 24));
        break;
    case 6:
        bmp->height = data;
        bmp->info[8] = (0xff & ((bmp->height) >> 0));
        bmp->info[9] = (0xff & ((bmp->height) >> 8));
        bmp->info[10] = (0xff & ((bmp->height) >> 16));
        bmp->info[11] = (0xff & ((bmp->height) >> 24));
        break;
    case 7:
        bmp->planes = data;
        bmp->info[12] = (0xff & ((bmp->planes) >> 0));
        bmp->info[13] = (0xff & ((bmp->planes) >> 8));
        break;
    case 8:
        bmp->bits_per_pixel = data;
        bmp->info[14] = (0xff & ((bmp->bits_per_pixel) >> 0));
        bmp->info[15] = (0xff & ((bmp->bits_per_pixel) >> 8));
        break;
    case 9:
        bmp->compression = data;
        bmp->info[16] = (0xff & ((bmp->compression) >> 0));
        bmp->info[17] = (0xff & ((bmp->compression) >> 8));
        bmp->info[18] = (0xff & ((bmp->compression) >> 16));
        bmp->info[19] = (0xff & ((bmp->compression) >> 24));
        break;
    case 10:
        bmp->bitmap_data_size = data;
        bmp->info[20] = (0xff & ((bmp->bitmap_data_size) >> 0));
        bmp->info[21] = (0xff & ((bmp->bitmap_data_size) >> 8));
        bmp->info[22] = (0xff & ((bmp->bitmap_data_size) >> 16));
        bmp->info[23] = (0xff & ((bmp->bitmap_data_size) >> 24));
        break;
    case 11:
        bmp->H = data;
        bmp->info[24] = (0xff & ((bmp->H) >> 0));
        bmp->info[25] = (0xff & ((bmp->H) >> 8));
        bmp->info[26] = (0xff & ((bmp->H) >> 16));
        bmp->info[27] = (0xff & ((bmp->H) >> 24));
        break;
    case 12:
        bmp->V = data;
        bmp->info[28] = (0xff & ((bmp->V) >> 0));
        bmp->info[29] = (0xff & ((bmp->V) >> 8));
        bmp->info[30] = (0xff & ((bmp->V) >> 16));
        bmp->info[31] = (0xff & ((bmp->V) >> 24));
        break;
    case 13:
        bmp->used_colors = data;
        bmp->info[32] = (0xff & ((bmp->used_colors) >> 0));
        bmp->info[33] = (0xff & ((bmp->used_colors) >> 8));
        bmp->info[34] = (0xff & ((bmp->used_colors) >> 16));
        bmp->info[35] = (0xff & ((bmp->used_colors) >> 24));
        break;
    case 14:
        bmp->important_colors = data;
        bmp->info[36] = (0xff & ((bmp->important_colors) >> 0));
        bmp->info[37] = (0xff & ((bmp->important_colors) >> 8));
        bmp->info[38] = (0xff & ((bmp->important_colors) >> 16));
        bmp->info[39] = (0xff & ((bmp->important_colors) >> 24));
        break;
    }
}

uint8_t *downsample(BMP *bmp, uint8_t *image, uint32_t image_pix, uint32_t width, uint32_t height)
{
    uint8_t *image_output = (uint8_t *)malloc(image_pix * sizeof(uint8_t));
    int tmp = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if ((x & 1) && (y & 1))
            {
                image_output[tmp] = image[x * width + y];
                tmp++;
            }
        }
    }
    BMP_Data_Reset(bmp, set_size, (width / 2) * (height / 2) + info_size + header_size + color_size);
    BMP_Data_Reset(bmp, set_width, (width / 2));
    BMP_Data_Reset(bmp, set_height, (height / 2));
    BMP_Data_Reset(bmp, set_bitmap_data_size, (width / 2) * (height / 2));
    return (image_output);
}

double MSE(uint8_t *image, uint8_t *new_image, uint32_t image_pix)
{
    int temp = 0, sum = 0;
    double mse = 0;
    for (int x = 0; x < image_pix; x++)
    {
        temp = (*(image + x)) - (*(new_image + x));
        sum += (temp * temp);
    }
    mse = (double)(sum) / (double)(image_pix);
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

double *normalization(double *image, uint32_t image_pix)
{
    double *normaliz_image = (double *)malloc(image_pix * sizeof(double));
    double max = *image, min = *image;
    for (int i = 0; i < image_pix; i++)
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
    for (int x = 0; x < image_pix; x++)
    {
        (*(normaliz_image + x)) = (((*(image + x)) - min) / (max - min)) * 255.0;
    }
    return normaliz_image;
}

double *image_sub(uint8_t *image, double *image_new, uint32_t image_pix)
{
    double *image_output = (double *)malloc(image_pix * sizeof(double));
    for (int x = 0; x < image_pix; x++)
    {
        *(image_output + x) = abs((int)(*(image_new + x) - ((double)(*(image + x)))));
    }
    return (image_output);
}

uint8_t *double_uint8(double *image, uint32_t image_pix)
{
    uint8_t *uint_image = (uint8_t *)(malloc(image_pix));
    for (uint32_t i = 0; i < image_pix; i++)
    {
        uint_image[i] = (uint8_t)round(abs((int)image[i]));
        // uint_image[i] = (uint8_t)round((int)image[i]);
    }
    return uint_image;
}

double *uint8_double(uint8_t *image, uint32_t image_pix)
{
    double *double_image = (double *)(malloc(sizeof(double) * image_pix));
    for (uint32_t i = 0; i < image_pix; i++)
    {
        double_image[i] = ((double)image[i]);
    }
    return double_image;
}

double *convolution(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width, uint32_t height)
{
    double *img_convolution = (double *)malloc(sizeof(double) * img_size);
    uint32_t convolution_x = width - (uint8_t)sqrt((double)con_mask_size);
    uint32_t convolution_y = height - (uint8_t)sqrt((double)con_mask_size);
    for (uint16_t y = 0; y < convolution_y; y++)
    {
        for (uint16_t x = 0; x < convolution_x; x++)
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

void gray_thresholding(double *img, uint32_t img_size, uint8_t level)
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

double *dilation(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width, uint32_t height)
{
    double *img_convolution = (double *)malloc(sizeof(double) * img_size);
    uint32_t convolution_x = width - (uint8_t)sqrt((double)con_mask_size);
    uint32_t convolution_y = height - (uint8_t)sqrt((double)con_mask_size);
    double sum = 0;
    for (uint16_t y = 0; y < convolution_y; y++)
    {
        for (uint16_t x = 0; x < convolution_x; x++)
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

double *erosion(double *img, double *con_mask, uint32_t img_size, uint32_t con_mask_size, uint32_t width, uint32_t height)
{
    double *img_convolution = (double *)malloc(sizeof(double) * img_size);
    uint32_t convolution_x = width - (uint8_t)sqrt((double)con_mask_size);
    uint32_t convolution_y = height - (uint8_t)sqrt((double)con_mask_size);
    double sum = 0;
    uint16_t mask_point = 0;
    for (uint16_t k = 0; k < con_mask_size; k++)
    {
        if (*(con_mask + k) == 1)
        {
            mask_point++;
        }
    }
    for (uint16_t y = 0; y < convolution_y; y++)
    {
        for (uint16_t x = 0; x < convolution_x; x++)
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
// double *image_Operation(uint8_t *image, double *image_new, int size)
// {
//     double *image_output = (double *)malloc(size * sizeof(double));
//     for (int x = 0; x < image_pixel; x++)
//     {
//         *(image_output + x) = abs((int)(*(image_new + x) - ((double)(*(image + x)))));
//     }
//     return (image_output);
// }
