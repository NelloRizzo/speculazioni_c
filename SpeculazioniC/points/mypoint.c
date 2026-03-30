#include "mypoint.h"
#include <stdlib.h>

Point *create_point(float x, float y)
{
    Point *p = malloc(sizeof(Point));
    if (!p)
        return NULL;
    p->x = x;
    p->y = y;
    return p;
}

Point *clone_point(const Point *p)
{
    if (!p)
        return NULL;
    return create_point(p->x, p->y);
}

void free_point(Point *p)
{
    if (p)
    {
        free(p);
    }
}

PointListItem *create_li(float x, float y)
{
    PointListItem *i = malloc(sizeof(PointListItem));
    if (!i)
        return NULL;
    i->next = NULL;
    i->p = create_point(x, y);
    if (!i->p)
    {
        free(i);
        return NULL;
    }
    return i;
}

PointListItem *create_li_from_point(const Point *p)
{
    if (!p)
        return NULL;
    return create_li(p->x, p->y);
}

void free_li(PointListItem *p)
{
    if (p)
    {
        free_point(p->p);
        free(p);
    }
}

PointList *create_list(void)
{
    PointList *l = malloc(sizeof(PointList));
    if (!l)
        return NULL;
    l->head = NULL;
    l->size = 0;
    l->tail = NULL;
    return l;
}

void free_list(PointList *list)
{
    if (!list)
        return;
    PointListItem *cursor = list->head;
    while (cursor)
    {
        PointListItem *next = cursor->next;
        free_li(cursor);
        cursor = next;
    }
    free(list);
}

int append_list_item(PointList *list, PointListItem *li)
{
    if (!li)
        return 0;
    if (list->tail)
        list->tail->next = li;
    if (!list->head)
        list->head = li;
    list->tail = li;
    list->size++;
    return 1;
}

void append_point(PointList *list, const Point *p)
{
    if (!list || !p)
        return;
    PointListItem *li = create_li_from_point(p);
    if (!append_list_item(list, li))
        free_li(li);
}

void append_point_from_coords(PointList *list, float x, float y)
{
    if (!list)
        return;
    PointListItem *li = create_li(x, y);
    if (!append_list_item(list, li))
        free_li(li);
}
void insert_point_after(PointList *list, const Point *p, PointListItem *previous)
{
    if (!list || !p)
        return;
    if (!previous)
    {
        append_point(list, p);
        return;
    }
    PointListItem *li = create_li_from_point(p);
    if (!li)
        return;

    li->next = previous->next;
    previous->next = li;
    list->size++;
}

void visit_list(PointList *list, void (*fn)(Point *p))
{
    PointListItem *cursor = list->head;
    while (cursor)
    {
        fn(cursor->p);
        cursor = cursor->next;
    }
}

void reduce_list(PointList *list, void (*fn)(const Point *p, void *accumulator), void *accumulator)
{
    PointListItem *cursor = list->head;
    while (cursor)
    {
        fn(cursor->p, accumulator);
        cursor = cursor->next;
    }
}
