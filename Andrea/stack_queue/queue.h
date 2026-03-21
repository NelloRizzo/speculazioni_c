#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct queue_node
{
    void *content;
    struct queue_node *next;
} queue_node;

typedef struct
{
    queue_node *head;
    queue_node *tail;
    size_t size;
} queue;

queue *queue_create(void);
void queue_free(queue *q, void (*free_fn)(void *));
void queue_enqueue(queue *q, void *item);
void *queue_dequeue(queue *q);
void *queue_peek(queue *q);
int queue_empty(queue *q);
int queue_size(queue *q);

#endif