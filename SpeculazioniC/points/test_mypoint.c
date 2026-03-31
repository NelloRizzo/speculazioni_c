#include "mypoint.h"
#include <stdio.h>

void print_point(Point *p)
{
    printf("P(%f, %f)\n", p->x, p->y);
}

struct sum_coords
{
    float sum_x;
    float sum_y;
};

void sum_all_coords(const Point *p, void *acc)
{
    struct sum_coords *sc = (struct sum_coords *)acc;
    sc->sum_x += p->x;
    sc->sum_y += p->y;
}

int main(void)
{
    PointList *li = create_point_list();
    if (!li)
    {
        fprintf(stderr, "Impossibile creare la lista di punti");
        return -1;
    }

    append_point_from_coords(li, 0, 1);
    PointListItem *a = append_point_from_coords(li, 1, 3);
    PointListItem *b = append_point_from_coords(li, 2, 7);
    append_point_from_coords(li, 3, 6);
    append_point_from_coords(li, 4, 5);
    PointListItem *c = append_point_from_coords(li, 5, 2);
    PointListItem *d = append_point_from_coords(li, 6, 3);
    append_point_from_coords(li, 7, 4);
    Point *ctrl = create_point(10, 10);
    lerp(li, a, b, 3);
    bezier(li, c, ctrl, d, 5);
    free_point(ctrl);
    printf("Punti in elenco:\n");
    visit_point_list(li, print_point);

    struct sum_coords sc = {0, 0};
    reduce_point_list(li, sum_all_coords, &sc);
    printf("Somme x: %f - Somme y: %f\n", sc.sum_x, sc.sum_y);
    Point *centroid = create_point(sc.sum_x / (float)li->size, sc.sum_y / (float)li->size);
    printf("Centroide: ");
    print_point(centroid);
    free_point(centroid);
    free_point_list(li);
}