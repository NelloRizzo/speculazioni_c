#include <stdlib.h>
#include "stack.h"

Stack *create_stack(void)
{
    Stack *s = malloc(sizeof(Stack));
    if (!s)
        return NULL;
    s->top = NULL;
    s->size = 0;
    return s;
}

void free_stack(Stack *s)
{
    if (!s)
        return;
    while (s->top)
    {
        stack_node *tmp = s->top;
        s->top = tmp->next;
        free(tmp);
    }
    free(s);
}

void push(Stack *s, Item data)
{
    if (!s)
        return;
    stack_node *n = malloc(sizeof(stack_node));
    if (!n)
        return;
    n->data = data;
    n->next = s->top;
    s->top = n;
    s->size++;
}

Item pop(Stack *s)
{
    if (!s || !s->top)
        return NULL;
    stack_node *tmp = s->top;
    Item data = tmp->data;
    s->top = tmp->next;
    free(tmp);
    s->size--;
    return data;
}

int is_empty(Stack *s)
{
    return !s || !s->top;
}
