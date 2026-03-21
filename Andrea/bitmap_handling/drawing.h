#ifndef DRAWING_H
#define DRAWING_H

#include "bitmap.h"

void draw_line(bitmap *bmp, int x0, int y0, int x1, int y1, pixel_color color);
void draw_line_alpha(bitmap *bmp, int x0, int y0, int x1, int y1, pixel_color color, uint8_t alpha);

void draw_rect(bitmap *bmp, int x, int y, int w, int h, pixel_color color);
void draw_rect_alpha(bitmap *bmp, int x, int y, int w, int h, pixel_color color, uint8_t alpha);

void fill_rect(bitmap *bmp, int x, int y, int w, int h, pixel_color color);
void fill_rect_alpha(bitmap *bmp, int x, int y, int w, int h, pixel_color color, uint8_t alpha);

void draw_circle(bitmap *bmp, int cx, int cy, int radius, pixel_color color);
void fill_circle(bitmap *bmp, int cx, int cy, int radius, pixel_color color);
void draw_ellipse(bitmap *bmp, int cx, int cy, int radius_x, int radius_y, pixel_color color);
void fill_ellipse(bitmap *bmp, int cx, int cy, int radius_x, int radius_y, pixel_color color);
void flood_fill(bitmap *bmp, int px, int py, pixel_color color);
void flood_fill_alpha(bitmap *bmp, int px, int py, pixel_color color, uint8_t alpha);
#endif