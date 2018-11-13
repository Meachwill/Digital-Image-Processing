#include "bmp_point.h"

uint8_t *BMP_READ(char *filename, BMP *bmp)
{
    FILE *image_org = image_org = fopen(filename, "rb");
    fread(bmp->header, sizeof(uint8_t), header_size, image_org);
    fread(bmp->info, sizeof(uint8_t), info_size, image_org);
    fread(bmp->color, sizeof(uint8_t), color_size, image_org);
    BMP_data(bmp);
    int image_pix = (bmp->width) * (bmp->width);
    uint8_t *img = (uint8_t *)malloc(image_pix * (sizeof(uint8_t)));
    fread(img, sizeof(uint8_t), image_pix, image_org);
    fclose(image_org);
    printf("read ok\n");
    return img;
}
void BMP_WRITE512(char *filename, BMP *bmp, uint8_t *image)
{
    int image_pix = 512 * 512;
    int size = header_size + info_size + color_size + image_pix;
    image512_output_struct image_output;
    memcpy(image_output.header_new, bmp->header, header_size);
    memcpy(image_output.info_new, bmp->info, info_size);
    memcpy(image_output.color_new, bmp->color, color_size);
    memcpy(image_output.image_new, image, image_pix);
    FILE *NewImageFile = fopen(filename, "wb");
    fwrite(image_output.header_new, 1, size, NewImageFile);
    fclose(NewImageFile);
    printf("write ok\n");
}
void BMP_WRITE256(char *filename, BMP *bmp, uint8_t *image)
{
    int image_pix = 256 * 256;
    int size = header_size + info_size + color_size + image_pix;
    image256_output_struct image_output;
    memcpy(image_output.header_new, bmp->header, header_size);
    memcpy(image_output.info_new, bmp->info, info_size);
    memcpy(image_output.color_new, bmp->color, color_size);
    memcpy(image_output.image_new, image, bmp->bitmap_data_size);
    FILE *NewImageFile = fopen(filename, "wb");
    fwrite(image_output.header_new, 1, size , NewImageFile);
    fclose(NewImageFile);
    printf("write ok\n");
}

void BMP_data(BMP *bmp)
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

// void BMP_data_reset(BMP *bmp){
//     //uint8_t new_header[14];
//     bmp->header[0] = (0xff & ((bmp->id) >> 0));
//     bmp->header[1] = (0xff & ((bmp->id) >> 8));
//     bmp->header[2] = (0xff & ((bmp->size) >> 0));
//     bmp->header[3] = (0xff & ((bmp->size) >> 8));
//     bmp->header[4] = (0xff & ((bmp->size) >> 16));
//     bmp->header[5] = (0xff & ((bmp->size) >> 24));
//     bmp->header[6] = (0xff & ((bmp->id) >> 8));
//     bmp->header[7] = (0xff & ((bmp->id) >> 8));
//     memcpy(bmp->header,bmp->id, header_size);
//     memcpy(bmp->info, bmp->bitmap_header_size, info_size);
// }
