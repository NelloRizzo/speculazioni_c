#include "list.h"

#include <stdlib.h>

typedef struct list_item
{
    void *data;
    struct list_item *next;
    struct list_item *previous;
} list_item;

struct list
{
    list_item *head;
    list_item *tail;
    int size;
};

List list_create(void)
{
    List list = malloc(sizeof(struct list));
    if (!list)
        return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_destroy(List l, void (*free_fn)(void *))
{
    if (!l)
        return;
    list_item *cursor = l->head;
    while (cursor)
    {
        list_item *next = cursor->next;
        if (free_fn)
            free_fn(cursor->data);
        free(cursor);
        cursor = next;
    }
    free(l);
}

list_item *create_li(void *data)
{
    list_item *item = malloc(sizeof(list_item));
    if (!item)
        return NULL;
    item->data = data;
    item->next = NULL;
    item->previous = NULL;
    return item;
}

void list_offer(List l, void *data)
{
    if (!l || !data)
        return;
    list_item *item = create_li(data);
    if (!item)
        return;
    item->next = l->head;
    l->head = item;
    if (!l->tail)
        l->tail = item;
    l->size++;
}

void list_append(List l, void *data)
{
    if (!l || !data)
        return;
    if (!l->head)
    {
        list_offer(l, data);
        return;
    }
    list_item *item = create_li(data);
    if (!item)
        return;
    item->previous = l->tail;
    l->tail->next = item;
    l->tail = item;
    l->size++;
}

void list_visit(List l, void (*fn)(void *))
{
    if (!fn || !l)
        return;
    list_item *cursor = l->head;
    while (cursor)
    {
        fn(cursor->data);
        cursor = cursor->next;
    }
}

void list_selection_sort(List l, int (*comparer)(void *, void *))
{
    if (!l || l->size < 2 || !comparer)
        return;

    list_item *i, *j, *min_node;

    // Selection sort
    for (i = l->head; i != NULL && i->next != NULL; i = i->next)
    {
        min_node = i;

        for (j = i->next; j != NULL; j = j->next)
        {
            // Se comparer restituisce un valore < 0, allora j->data è "più piccolo" di min_node->data
            if (comparer(j->data, min_node->data) < 0)
            {
                min_node = j;
            }
        }

        if (min_node != i)
        {
            void *temp = i->data;
            i->data = min_node->data;
            min_node->data = temp;
        }
    }
}
