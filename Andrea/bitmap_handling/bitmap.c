#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

#define SIGNATURE 0x4D42 // 'BM' (little endian MB)

typedef struct __attribute__((packed))
{
    uint16_t signature;
    uint32_t size;
    uint32_t reserved;
    uint32_t offset;
} file_header;

typedef struct __attribute__((packed))
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t colors;
    uint16_t bit_per_pixel;
    uint32_t compression;
    uint32_t pixel_data_size;
    uint32_t pixel_per_meter_x;
    uint32_t pixel_per_meter_y;
    uint32_t palette;
    uint32_t primary_colors;
} info_header;

bitmap *bitmap_create(int width, int height)
{
    bitmap *bmp = malloc(sizeof(bitmap));
    if (!bmp)
        return NULL;
    bmp->height = height;
    bmp->width = width;
    bmp->pixels = calloc((size_t)width * (size_t)height, sizeof(pixel_color));
    if (!bmp->pixels)
    {
        free(bmp);
        return NULL;
    }
    return bmp;
}

void bitmap_free(bitmap *bmp)
{
    if (!bmp)
        return;
    free(bmp->pixels);
    free(bmp);
}

pixel_color *get_pixel(bitmap *bmp, int x, int y)
{
    if (!bmp)
        return NULL;
    if (x < 0 || x >= bmp->width || y < 0 || y >= bmp->height)
        return NULL;
    int pos = y * bmp->width + x;
    return bmp->pixels + pos;
}

void set_pixel(bitmap *bmp, int x, int y, pixel_color px)
{
    if (!bmp)
        return;
    if (x < 0 || x >= bmp->width || y < 0 || y >= bmp->height)
        return;
    int pos = y * bmp->width + x;
    bmp->pixels[pos] = px;
}

uint8_t blend_color(int src, int dest, int a)
{
    return (uint8_t)((src * a + dest * (255 - a)) / 255);
}

void draw_pixel_alpha(bitmap *bmp, int x, int y, pixel_color color, uint8_t alpha)
{
    pixel_color dest = *get_pixel(bmp, x, y);
    dest = (pixel_color){blend_color(color.red, dest.red, alpha), blend_color(color.green, dest.green, alpha), blend_color(color.blue, dest.blue, alpha)};
    set_pixel(bmp, x, y, dest);
}

void bitmap_save(bitmap *bmp, const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        fprintf(stderr, "Unable to open file %s for writing.\n", filename);
        return;
    }
    uint16_t signature = SIGNATURE;
    int row_size = (bmp->width * 3 + 3) & ~3; // arrotonda al multiplo di 4
    uint32_t pixel_data_size = (uint32_t)(bmp->height * row_size);
    uint32_t file_size = 14 + 40 + pixel_data_size;
    file_header header_f = {signature, file_size, 0, 54};
    info_header header_i = {40, (uint32_t)bmp->width, (uint32_t)bmp->height, 1, 24, 0, pixel_data_size, 0, 0, 0, 0};
    fwrite(&header_f, sizeof(file_header), 1, f);
    fwrite(&header_i, sizeof(info_header), 1, f);
    uint8_t padding[3] = {0, 0, 0};      // padding per allineare a multipli di 3 le righe
    int pad = row_size - bmp->width * 3; // numero di 'padding' da aggiungere per riga
    for (int y = bmp->height - 1; y >= 0; --y)
    {
        for (int x = 0; x < bmp->width; ++x)
        {
            pixel_color *px = &bmp->pixels[y * bmp->width + x];
            fputc(px->blue, f);
            fputc(px->green, f);
            fputc(px->red, f);
        }
        fwrite(padding, 1, (size_t)pad, f);
    }
    fclose(f);
}

bitmap *bitmap_load(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
        return NULL;
    file_header fh;
    fread(&fh, sizeof(file_header), 1, f);
    if (fh.signature != SIGNATURE)
    {
        fclose(f);
        return NULL;
    }
    info_header ih;
    fread(&ih, sizeof(info_header), 1, f);
    if (ih.compression != 0)
    {
        fprintf(stderr, "Compression method not supported\n");
        fclose(f);
        return NULL;
    }
    int width = (int)ih.width;
    int height = (int)ih.height;
    if (ih.bit_per_pixel != 24)
    {
        fprintf(stderr, "Only 24-bit BMP supported\n");
        fclose(f);
        return NULL;
    }
    fseek(f, fh.offset, SEEK_SET);
    bitmap *bmp = bitmap_create(width, height);
    if (!bmp)
    {
        fclose(f);
        return NULL;
    }
    int row_size = (bmp->width * 3 + 3) & ~3; // arrotonda al multiplo di 4
    uint8_t padding[3] = {0, 0, 0};           // padding per allineare a multipli di 3 le righe
    int pad = row_size - bmp->width * 3;      // numero di 'padding' da aggiungere per riga
    uint8_t r, g, b;
    for (int y = bmp->height - 1; y >= 0; --y)
    {
        for (int x = 0; x < bmp->width; ++x)
        {
            b = (uint8_t)fgetc(f);
            g = (uint8_t)fgetc(f);
            r = (uint8_t)fgetc(f);
            set_pixel(bmp, x, y, (pixel_color){r, g, b});
        }
        fread(padding, 1, (size_t)pad, f);
    }
    fclose(f);
    return bmp;
}

int is_same_color(pixel_color a, pixel_color b) { return a.blue == b.blue && a.green == b.green && a.red == b.red; }
