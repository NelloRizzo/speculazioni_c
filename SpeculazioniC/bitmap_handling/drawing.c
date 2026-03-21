#include "drawing.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int x;
    int y;
} point;

point *make_point(int px, int py)
{
    point *p = malloc(sizeof(point));
    if (!p)
        return NULL;
    p->x = px;
    p->y = py;
    return p;
}

void draw_line(bitmap *bmp, int x0, int y0, int x1, int y1, pixel_color color)
{
    if (!bmp)
        return;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1; // direzione x
    int sy = (y0 < y1) ? 1 : -1; // direzione y
    int err = dx - dy;

    while (1)
    {
        set_pixel(bmp, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}
void draw_line_alpha(bitmap *bmp, int x0, int y0, int x1, int y1, pixel_color color, uint8_t alpha)
{
    if (!bmp)
        return;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1; // direzione x
    int sy = (y0 < y1) ? 1 : -1; // direzione y
    int err = dx - dy;

    while (1)
    {
        draw_pixel_alpha(bmp, x0, y0, color, alpha);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_rect(bitmap *bmp, int x, int y, int w, int h, pixel_color color)
{
    draw_line(bmp, x, y, x + w - 1, y, color);
    draw_line(bmp, x, y, x, y + h - 1, color);
    draw_line(bmp, x + w - 1, y, x + w - 1, y + h - 1, color);
    draw_line(bmp, x, y + h - 1, x + w - 1, y + h - 1, color);
}
void draw_rect_alpha(bitmap *bmp, int x, int y, int w, int h, pixel_color color, uint8_t alpha)
{
    draw_line_alpha(bmp, x, y, x + w - 1, y, color, alpha);
    draw_line_alpha(bmp, x, y, x, y + h - 1, color, alpha);
    draw_line_alpha(bmp, x + w - 1, y, x + w - 1, y + h - 1, color, alpha);
    draw_line_alpha(bmp, x, y + h - 1, x + w - 1, y + h - 1, color, alpha);
}

void fill_rect(bitmap *bmp, int x, int y, int w, int h, pixel_color color)
{
    for (int r = y; r < y + h; ++r)
    {
        draw_line(bmp, x, r, x + w - 1, r, color);
    }
}

void fill_rect_alpha(bitmap *bmp, int x, int y, int w, int h, pixel_color color, uint8_t alpha)
{
    for (int r = y; r < y + h; ++r)
    {
        draw_line_alpha(bmp, x, r, x + w - 1, r, color, alpha);
    }
}

void draw_circle(bitmap *bmp, int cx, int cy, int radius, pixel_color color)
{
    draw_ellipse(bmp, cx, cy, radius, radius, color);
}
void draw_ellipse(bitmap *bmp, int cx, int cy, int radius_x, int radius_y, pixel_color color)
{
    if (!bmp)
        return;
    int x = 0, y = radius_y;
    int rx2 = radius_x * radius_x;
    int ry2 = radius_y * radius_y;
    int p1 = ry2 - rx2 * radius_y + rx2 / 4;
    while (2 * ry2 * x <= 2 * rx2 * y)
    {
        set_pixel(bmp, x + cx, y + cy, color);
        set_pixel(bmp, -x + cx, y + cy, color);
        set_pixel(bmp, x + cx, -y + cy, color);
        set_pixel(bmp, -x + cx, -y + cy, color);
        x++;
        if (p1 < 0)
        {
            p1 += 2 * ry2 * x + ry2;
        }
        else
        {
            y--;
            p1 += 2 * ry2 * x - 2 * rx2 * y + ry2;
        }
    }
    int p2 = ry2 * (2 * x + 1) * (2 * x + 1) / 4 + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y > 0)
    {
        set_pixel(bmp, x + cx, y + cy, color);
        set_pixel(bmp, -x + cx, y + cy, color);
        set_pixel(bmp, x + cx, -y + cy, color);
        set_pixel(bmp, -x + cx, -y + cy, color);
        y--;
        if (p2 > 0)
        {
            p2 += rx2 - 2 * rx2 * y;
        }
        else
        {
            x++;
            p2 += 2 * ry2 * x - 2 * rx2 * y + rx2;
        }
    }
}
void fill_ellipse(bitmap *bmp, int cx, int cy, int radius_x, int radius_y, pixel_color color)
{
    if (!bmp)
        return;
    int x = 0, y = radius_y;
    int rx2 = radius_x * radius_x;
    int ry2 = radius_y * radius_y;
    int p1 = ry2 - rx2 * radius_y + rx2 / 4;
    while (2 * ry2 * x <= 2 * rx2 * y)
    {
        draw_line(bmp, x + cx, y + cy, -x + cx, y + cy, color);
        draw_line(bmp, x + cx, -y + cy, -x + cx, -y + cy, color);
        x++;
        if (p1 < 0)
        {
            p1 += 2 * ry2 * x + ry2;
        }
        else
        {
            y--;
            p1 += 2 * ry2 * x - 2 * rx2 * y + ry2;
        }
    }
    int p2 = ry2 * (2 * x + 1) * (2 * x + 1) / 4 + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y >= 0)
    {
        draw_line(bmp, x + cx, y + cy, -x + cx, y + cy, color);
        draw_line(bmp, x + cx, -y + cy, -x + cx, -y + cy, color);
        y--;
        if (p2 > 0)
        {
            p2 += rx2 - 2 * rx2 * y;
        }
        else
        {
            x++;
            p2 += 2 * ry2 * x - 2 * rx2 * y + rx2;
        }
    }
}

static void enqueue_if_valid(queue *q, bitmap *bmp, int x, int y, pixel_color target, pixel_color color, uint8_t alpha)
{
    if (x < 0 || x >= bmp->width || y < 0 || y >= bmp->height)
    {
        return;
    }
    pixel_color *c = get_pixel(bmp, x, y);
    if (!is_same_color(*c, target))
        return;
    draw_pixel_alpha(bmp, x, y, color, alpha);
    point *p = make_point(x, y);
    if (p)
        queue_enqueue(q, p);
}

void flood_fill(bitmap *bmp, int px, int py, pixel_color color)
{
    if (!bmp)
        return;
    pixel_color target = *get_pixel(bmp, px, py);
    if (is_same_color(target, color))
        return;
    queue *q = queue_create();
    if (!q)
        return;
    set_pixel(bmp, px, py, color);
    queue_enqueue(q, make_point(px, py));
    while (!queue_empty(q))
    {
        point *p = queue_dequeue(q);
        enqueue_if_valid(q, bmp, p->x + 1, p->y, target, color, 255);
        enqueue_if_valid(q, bmp, p->x - 1, p->y, target, color, 255);
        enqueue_if_valid(q, bmp, p->x, p->y + 1, target, color, 255);
        enqueue_if_valid(q, bmp, p->x, p->y - 1, target, color, 255);
        free(p);
    }
    queue_free(q, free);
}

void flood_fill_alpha(bitmap *bmp, int px, int py, pixel_color color, uint8_t alpha)
{
    if (!bmp)
        return;
    pixel_color target = *get_pixel(bmp, px, py);
    if (is_same_color(target, color))
        return;
    queue *q = queue_create();
    if (!q)
        return;
    draw_pixel_alpha(bmp, px, py, color, alpha);
    queue_enqueue(q, make_point(px, py));
    while (!queue_empty(q))
    {
        point *p = queue_dequeue(q);
        enqueue_if_valid(q, bmp, p->x + 1, p->y, target, color, alpha);
        enqueue_if_valid(q, bmp, p->x - 1, p->y, target, color, alpha);
        enqueue_if_valid(q, bmp, p->x, p->y + 1, target, color, alpha);
        enqueue_if_valid(q, bmp, p->x, p->y - 1, target, color, alpha);
        free(p);
    }
    queue_free(q, free);
}

void fill_circle(bitmap *bmp, int cx, int cy, int radius, pixel_color color)
{
    fill_ellipse(bmp, cx, cy, radius, radius, color);
}