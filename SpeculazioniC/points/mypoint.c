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

PointListItem *create_point_li(float x, float y)
{
    PointListItem *i = malloc(sizeof(PointListItem));
    if (!i)
        return NULL;
    i->next = NULL;
    i->surveyed = 1;
    i->p = create_point(x, y);
    if (!i->p)
    {
        free(i);
        return NULL;
    }
    return i;
}

PointListItem *create_point_li_from_point(const Point *p)
{
    if (!p)
        return NULL;
    return create_point_li(p->x, p->y);
}

void free_li(PointListItem *p)
{
    if (p)
    {
        free_point(p->p);
        free(p);
    }
}

PointList *create_point_list(void)
{
    PointList *l = malloc(sizeof(PointList));
    if (!l)
        return NULL;
    l->head = NULL;
    l->size = 0;
    l->tail = NULL;
    return l;
}

void free_point_list(PointList *list)
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
    PointListItem *li = create_point_li_from_point(p);
    if (!append_list_item(list, li))
        free_li(li);
}

PointListItem *append_point_from_coords(PointList *list, float x, float y)
{
    if (!list)
        return NULL;
    PointListItem *li = create_point_li(x, y);
    if (!append_list_item(list, li))
    {
        free_li(li);
        return NULL;
    }
    return li;
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
    PointListItem *li = create_point_li_from_point(p);
    if (!li)
        return;

    li->next = previous->next;
    previous->next = li;
    list->size++;
}

void visit_point_list(PointList *list, void (*fn)(Point *p))
{
    PointListItem *cursor = list->head;
    while (cursor)
    {
        fn(cursor->p);
        cursor = cursor->next;
    }
}

void reduce_point_list(PointList *list, void (*fn)(const Point *p, void *accumulator), void *accumulator)
{
    PointListItem *cursor = list->head;
    while (cursor)
    {
        fn(cursor->p, accumulator);
        cursor = cursor->next;
    }
}

int lerp(PointList *list, PointListItem *a, PointListItem *b, int n_points)
{
    if (!list || !a || !b || n_points <= 0 || a->next != b) // a->next != b impedisce la creazione tra punti non consecutivi
        return 0;

    int count = 0;
    float dx = b->p->x - a->p->x;
    float dy = b->p->y - a->p->y;
    for (int k = 1; k <= n_points; k++)
    {
        float t = (float)k / (float)(n_points + 1);
        float x = a->p->x + t * dx;
        float y = a->p->y + t * dy;

        PointListItem *new_li = create_point_li(x, y);
        if (!new_li)
            return count;
        new_li->surveyed = 0;
        new_li->next = b;
        a->next = new_li;
        a = new_li;
        list->size++;
        count++;
    }
    return count;
}

int bezier(PointList *list, PointListItem *a, const Point *control_point, PointListItem *b, int n_points)
{
    if (!list || !a || !b || !control_point || n_points <= 0 || a->next != b)
        return 0;

    int count = 0;
    for (int k = 1; k <= n_points; k++)
    {
        float t = (float)k / (float)(n_points + 1);
        float u = 1.0f - t;
        float x = u * u * a->p->x + 2 * u * t * control_point->x + t * t * b->p->x;
        float y = u * u * a->p->y + 2 * u * t * control_point->y + t * t * b->p->y;

        PointListItem *new_li = create_point_li(x, y);
        if (!new_li)
            return count;

        new_li->surveyed = 0;
        new_li->next = b;
        a->next = new_li;
        a = new_li;
        list->size++;
        count++;
    }
    return count;
}