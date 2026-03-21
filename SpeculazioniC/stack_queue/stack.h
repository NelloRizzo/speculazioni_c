#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct stack_node
{
    void *content;
    struct stack_node *next;
} stack_node;

typedef struct
{
    stack_node *head;
    size_t size;
} stack;

stack *stack_create(void);
void stack_free(stack *s, void (*free_fn)(void *));
void stack_push(stack *s, void *item);
void *stack_pop(stack *s);
int stack_empty(const stack *s);

#endif