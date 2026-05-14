#include <stdlib.h>
#include "queue.h"

Queue *create_queue(void)
{
    Queue *q = malloc(sizeof(Queue));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void free_queue(Queue *q)
{
    if (!q)
        return;
    while (q->head)
    {
        queue_node *tmp = q->head;
        q->head = tmp->next;
        free(tmp);
    }
    free(q);
}

void enqueue(Queue *q, Item data)
{
    if (!q)
        return;
    queue_node *n = malloc(sizeof(queue_node));
    if (!n)
        return;
    n->data = data;
    n->next = NULL;

    if (!q->tail)
    {
        q->head = n;
        q->tail = n;
    }
    else
    {
        q->tail->next = n;
        q->tail = n;
    }
    q->size++;
}

Item dequeue(Queue *q)
{
    if (!q || !q->head)
        return NULL;
    queue_node *tmp = q->head;
    Item data = tmp->data;
    q->head = tmp->next;
    if (!q->head)
        q->tail = NULL;
    free(tmp);
    q->size--;
    return data;
}

int is_empty_q(Queue *q)
{
    return !q || !q->head;
}
