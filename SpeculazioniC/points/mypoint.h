#ifndef MY_POINT_H
#define MY_POINT_H

#include <stddef.h>

typedef struct point
{
    float x;
    float y;
} Point;

typedef struct point_list_item
{
    Point *p;
    struct point_list_item *next;
    // indica se il punto è calcolato tramite interpolazione (surveyed == 0)
    int surveyed;
} PointListItem;

typedef struct
{
    PointListItem *head;
    PointListItem *tail;
    size_t size;
} PointList;

Point *create_point(float x, float y);
Point *clone_point(const Point *p);
void free_point(Point *p);

PointListItem *create_point_li(float x, float y);
PointListItem *create_point_li_from_point(const Point *p);
void free_li(PointListItem *p);

PointList *create_point_list(void);
void free_point_list(PointList *list);
void append_point(PointList *list, const Point *p);
PointListItem *append_point_from_coords(PointList *list, float x, float y);
void insert_point_after(PointList *list, const Point *p, PointListItem *previous);
void visit_point_list(PointList *list, void (*fn)(Point *p));
void reduce_point_list(PointList *list, void (*fn)(const Point *p, void *accumulator), void *accumulator);
/*
Interpolazione lineare. Aggiunge, tra due punti contigui a e b, n_points  equidistanti sul segmento che unisce a con b.
*/
int lerp(PointList *list, PointListItem *a, PointListItem *b, int n_points);
/*
Interpolazione di Bezier. Aggiunge, tra due punti contigui a e b, n_points equidistanti secondo la funzione di Bezier, 
utilizzando un punto di controllo (esterno alla lista).
*/
int bezier(PointList *list, PointListItem *a, const Point *control_point, PointListItem *b, int n_points);

#endif