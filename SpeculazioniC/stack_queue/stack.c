#include <stddef.h>
#include <stdlib.h>
#include "stack.h"

stack *stack_create(void)
{
    stack *s = malloc(sizeof(stack));
    if (!s)
        return NULL;

    s->head = NULL;
    s->size = 0;
    return s;
}

void stack_free(stack *s, void (*free_fn)(void *))
{
    if (!s)
        return;

    while (s->head)
    {
        void *data = stack_pop(s);
        if (free_fn)
            free_fn(data);
    }
    free(s);
}

void stack_push(stack *s, void *item)
{
    if (!s)
        return;

    stack_node *si = malloc(sizeof(stack_node));
    if (!si)
        return;

    si->content = item;
    si->next = s->head;
    s->head = si;
    s->size++;
}

void *stack_pop(stack *s)
{
    if (!s || !s->head)
        return NULL;
    stack_node *item = s->head;
    s->head = item->next;
    void *content = item->content;
    free(item);
    s->size--;
    return content;
}

int stack_empty(const stack *s)
{
    return !s || !s->head;
}