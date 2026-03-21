#include <stdio.h>
#include "stack.h"
#include "queue.h"

void stack_queue_full_integration_test(void)
{
    stack *s = stack_create();

    stack_push(s, "Uno");
    stack_push(s, "Due");
    stack_push(s, "Tre");

    while (!stack_empty(s))
    {
        printf("%s\n", (char *)stack_pop(s));
    }

    stack_free(s, NULL);

    queue *q = queue_create();
    queue_enqueue(q, "Uno");
    queue_enqueue(q, "Due");
    queue_enqueue(q, "Tre");
    while (!queue_empty(q))
    {
        printf("%s\n", (char *)queue_dequeue(q));
    }

    queue_free(q, NULL);
}