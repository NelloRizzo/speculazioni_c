#include "queue.h"
#include <stdlib.h>

queue *queue_create(void)
{
    queue *q = malloc(sizeof(queue));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    return q;
}

void queue_free(queue *q, void (*free_fn)(void *))
{
    if (!q)
        return;

    while (q->head)
    {
        void *data = queue_dequeue(q);
        if (free_fn)
            free_fn(data);
    }
    free(q);
}

void queue_enqueue(queue *q, void *item)
{
    if (!q)
        return;

    queue_node *qi = malloc(sizeof(queue_node));
    if (!qi)
        return;

    qi->content = item;
    qi->next = NULL;

    if (!q->head)
    {
        q->head = qi;
    }
    else
    {
        q->tail->next = qi;
    }
    q->tail = qi;
    q->size++;
}

void *queue_dequeue(queue *q)
{
    if (!q || !q->head)
        return NULL;
    queue_node *item = q->head;
    q->head = item->next;
    void *content = item->content;

    if (!q->head)
        q->tail = NULL;

    free(item);
    q->size--;
    return content;
}

int queue_empty(queue *q)
{
    return !q || !q->head;
}

int queue_size(queue *q)
{
    return (int)q->size;
}

void *queue_peek(queue *q)
{
    if (!q || !q->head)
        return NULL;

    return q->head->content;
}