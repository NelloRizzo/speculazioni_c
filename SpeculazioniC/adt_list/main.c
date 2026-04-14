#include <stdio.h>

#include "list.h"

void print(void *item)
{
    printf("%d\n", *(int *)item);
}

int rec_power(int a, int b)
{
    if (b == 0)
        return 1;
    return a * rec_power(a, b - 1);
}

int compare(void *a, void *b)
{
    int x = *(int *)a, y = *(int *)b;
    if (x == y)
        return 0;
    return x > y ? 1 : -1;
}

int main(void)
{
    List l = list_create();
    int a = 0;
    int b = 1;
    int c = 2;
    list_offer(l, &a);
    list_offer(l, &b);
    list_append(l, &c);
    printf("Lista originaria:\n");
    list_visit(l, print);
    list_selection_sort(l, compare);
    printf("Lista ordinata:\n");
    list_visit(l, print);
    list_destroy(l, NULL);
    return 0;
}