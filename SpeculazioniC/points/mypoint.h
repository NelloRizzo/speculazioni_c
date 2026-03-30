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

PointListItem *create_li(float x, float y);
PointListItem *create_li_from_point(const Point *p);
void free_li(PointListItem *p);

PointList *create_list(void);
void free_list(PointList *list);
void append_point(PointList *list, const Point *p);
void append_point_from_coords(PointList *list, float x, float y);
void insert_point_after(PointList *list, const Point *p, PointListItem *previous);
void visit_list(PointList *list, void (*fn)(Point *p));
void reduce_list(PointList *list, void (*fn)(const Point *p, void *accumulator), void *accumulator);

#endif