#include <stdio.h>

#include "stack.h"

#define MAX_EXPR_LEN 125
#define STACK_ERROR -2
#define IS_BALANCED -1

void print(void *data)
{
    char *c = (char *)data;
    printf("%c\n", *c);
}

int related(char closed)
{
    return closed == ')' ? '(' : (closed == ']' ? '[' : (closed == '}' ? '{' : 'x'));
}

int is_opened(char c)
{
    return c == '(' || c == '[' || c == '{';
}
int is_closed(char c)
{
    return c == ')' || c == ']' || c == '}';
}

int check_expression(char *expr)
{
    Stack *s = stack_create();
    if (!s)
        return STACK_ERROR;
    char *cursor = expr;
    int position = 0;
    while (*cursor)
    {
        position++;
        if (is_closed(*cursor) || is_opened(*cursor))
        {
            if (*cursor == '(' || *cursor == '[' || *cursor == '{')
            {
                stack_push(s, cursor);

                printf("LOG: Parentesi aperta: %c\n", *cursor);
            }
            else
            {
                char *last_opened = (char *)stack_pop(s);
                printf("LOG: Parentesi chiusa: %c - Ultima aperta: %c\n", *cursor, *last_opened);

                if (!last_opened || *last_opened != related(*cursor))
                {
                    stack_free(s);
                    return position;
                }
            }
        }
        cursor++;
    }
    int result = stack_size(s) ? position : IS_BALANCED;
    stack_free(s);
    return result;
}

int main(void)
{
    printf("Inserisci l'espressione: ");
    char expression[MAX_EXPR_LEN];
    scanf("%s", expression);
    int check = check_expression(expression);
    if (check == STACK_ERROR)
    {
        printf("Impossibile creare lo stack\n");
        return (-1);
    }

    if (check == IS_BALANCED)
    {
        printf("L'espressione è bilanciata\n");
    }
    else
    {
        printf("L'espressione non è bilanciata (posizione %d)\n", check);
    }
    return 0;
}