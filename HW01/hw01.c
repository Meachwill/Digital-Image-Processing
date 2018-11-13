#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
typedef long uint32;
typedef unsigned short int uint16;
typedef unsigned char byte;

#define r 2
#define c 1
#define header_size 14
#define info_size 40
#define color_size 1024
#define image_size 262144
#define U16(x) ((unsigned short) (x))
#define U32(x) ((int) (x))
#define b2u16(data,offset) (U16(data[offset]) | U16(data[offset+1])<<8)
#define b2u32(data,offset) (U32(data[offset]) | U32(data[offset+1])<<8 | U32(data[offset+2])<<16 | U32(data[offset+3])<<24)

typedef struct BMP{
    byte header[header_size];
    byte info[info_size];
    //header
    uint16  id;
    uint32 size;
    uint32 reserve;
    uint32 bitmap_data_offset;
    //info
    uint32 bitmap_header_size;
    uint32 width;
    uint32 height;
    uint16 planes;
    uint16 bits_per_pixel;
    uint32 compression;
    uint32 bitmap_data_size;
    uint32 H;
    uint32 V;
    uint32 used_colors;
    uint32 important_colors;

}BMP;

typedef struct image_new{
    byte header_TEST[header_size];
    byte info_TEST[info_size];
    byte color_TEST[color_size];
    byte image_cod[image_size];
}image_new;

int main(){
    int k=0;
    BMP bmp;
    image_new IMAGE_NEW;
    //BMP bmp_new;
    byte image[262144]={0};
    byte image_new[262144]={0};
    byte image_out[263222]={0};
    byte HEADER[14];
    byte INFO[40];
    byte COLOR[1024];
    FILE *image_org,*gray,*image_new_output,*image_hisogram;
    //image in
    image_org=fopen("test.bmp","rb");
    //hitogram out
    gray=fopen("_test_gray_dis.txt","w+t");
    //fread binary data
    fread(HEADER,1,header_size,image_org);
    fread(INFO,1,info_size,image_org);
    fread(COLOR,1,color_size,image_org);
    fread(image, 1, image_size, image_org);
    //storge in struct
    memcpy(bmp.header,HEADER,header_size);
    memcpy(bmp.info,INFO,info_size);
    //print
    bmp.size=b2u32((bmp.header),2);
    fclose(image_org);
    printf("size=%ld\n",bmp.size);
    pr intf("%d\n", sizeof(r));
    for (int i = 0; i < 262144; i++)
    {
        fprintf(gray, "%d\t", image[i]);
    }
    //new image
    for(int i=0;i<262144;i++){
        image_new[i]=(char)((c*pow((float)(image[i]/255.0),r))*255.0);
    }
    //new image output
    memcpy(IMAGE_NEW.header_TEST,HEADER, header_size);
    memcpy(IMAGE_NEW.info_TEST,INFO,info_size);
    memcpy(IMAGE_NEW.color_TEST,COLOR,color_size);
    memcpy(IMAGE_NEW.image_cod, image_new,image_size);
    image_new_output = fopen("test_image_r4.bmp", "wb");
    fwrite(IMAGE_NEW.header_TEST,1,sizeof(image_out),image_new_output);
    fclose(image_new_output);
    image_hisogram=fopen("test_image_histogram4.txt","w+t");
    for (int i = 0; i < 262144; i++)
    {
        fprintf(image_hisogram, "%d\t", IMAGE_NEW.header_TEST[i]);
    }
    fclose(image_hisogram);
    return 0;
}