#ifndef STACK_H
#define STACK_H

#include "item.h"

typedef struct stack_node {
    Item data;
    struct stack_node *next;
} stack_node;

typedef struct {
    stack_node *top;
    int size;
} Stack;

Stack *create_stack(void);
void free_stack(Stack *s);
void push(Stack *s, Item data);
Item pop(Stack *s);
int is_empty(Stack *s);

#endif
