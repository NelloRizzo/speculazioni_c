#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_color;

typedef struct
{
    pixel_color *pixels;
    int width;
    int height;
} bitmap;

int is_same_color(pixel_color a, pixel_color b);

bitmap *bitmap_create(int width, int height);
void bitmap_free(bitmap *bmp);
pixel_color *get_pixel(bitmap *bmp, int x, int y);
uint8_t blend_color(int src, int dest, int a);

void set_pixel(bitmap *bmp, int x, int y, pixel_color px);
void draw_pixel_alpha(bitmap *bmp, int x, int y, pixel_color color, uint8_t alpha);
void bitmap_save(bitmap *bmp, const char *filename);
bitmap *bitmap_load(const char *filename);

#endif