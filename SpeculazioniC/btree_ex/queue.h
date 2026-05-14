#ifndef QUEUE_H
#define QUEUE_H

#include "item.h"

typedef struct queue_node {
    Item data;
    struct queue_node *next;
} queue_node;

typedef struct {
    queue_node *head;
    queue_node *tail;
    int size;
} Queue;

Queue *create_queue(void);
void free_queue(Queue *q);
void enqueue(Queue *q, Item data);
Item dequeue(Queue *q);
int is_empty_q(Queue *q);

#endif
