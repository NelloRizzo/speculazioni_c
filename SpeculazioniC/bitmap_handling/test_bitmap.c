#include "bitmap.h"
#include "drawing.h"
#include "known_colors.h"
#include <math.h>

#define PI 3.14159265358979

void bitmap_full_integration_test(void)
{
    bitmap *bmp = bitmap_create(500, 500);
    fill_rect(bmp, 0, 0, 500, 500, WHITE);
    draw_line(bmp, 0, 0, 500, 500, GREEN);
    draw_line(bmp, 500, 0, 0, 500, GREEN);
    fill_ellipse(bmp, 250, 250, 150, 100, RED);
    fill_circle(bmp, 250, 250, 103, WHITE);
    fill_circle(bmp, 250, 250, 100, BLUE);

    int r_out = 100;
    int r_in = 75;
    int x[10], y[10];
    int cx = 250, cy = 250;
    for (int i = 0; i < 10; i++)
    {
        double angle = i * PI / 5 - PI / 2;     // 10 vertici, ogni 36 gradi
        double r = (i % 2 == 0) ? r_out : r_in; // alterna esterno/interno
        x[i] = cx + (int)(r * cos(angle));
        y[i] = cy + (int)(r * sin(angle));
    }
    for (int i = 1; i < 10; i++)
    {
        draw_line(bmp, x[i - 1], y[i - 1], x[i], y[i], BLACK);
    }
    draw_line(bmp, x[0], y[0], x[9], y[9], BLACK);
    flood_fill(bmp, cx, cy, YELLOW);
    bitmap_save(bmp, "test.bmp");
    bitmap_free(bmp);
}

int main(void)
{
    bitmap_full_integration_test();
    return 0;
}