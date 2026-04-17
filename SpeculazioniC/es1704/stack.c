#include "stack.h"

#include <stdlib.h>

struct stack_item
{
    void *data;
    struct stack_item *next;
};

struct stack
{
    struct stack_item *head;
    int size;
};

Stack *stack_create(void)
{
    Stack *s = malloc(sizeof(Stack));
    if (!s)
        return NULL;
    s->head = NULL;
    s->size = 0;
    return s;
}

void *stack_pop(Stack *s)
{
    if (!s || !s->head)
        return NULL;
    struct stack_item *top = s->head;
    void *data = top->data;
    s->head = top->next;
    s->size--;
    free(top);
    return data;
}

void stack_push(Stack *s, void *data)
{
    if (!s)
        return;
    struct stack_item *item = malloc(sizeof(struct stack_item));
    if (!item)
        return;
    item->data = data;
    item->next = s->head;
    s->head = item;
    s->size++;
}

int stack_size(Stack *s)
{
    if (!s)
        return 0;
    return s->size;
}

int stack_empty(Stack *s)
{
    return !s || !s->size;
}

void stack_free(Stack *s)
{
    if (!s)
        return;
    while (s->size)
    {
        stack_pop(s);
    }
    free(s);
}

void stack_visit(Stack *s, visit_fn visitor)
{
    if (!s || !visitor)
        return;
    struct stack_item *cursor = s->head;
    while (cursor)
    {
        visitor(cursor->data);
        cursor = cursor->next;
    }
}
