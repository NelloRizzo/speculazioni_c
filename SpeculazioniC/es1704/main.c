#include <stdio.h>

#include "stack.h"

#define MAX_EXPR_LEN 125
#define STACK_ERROR -2
#define IS_BALANCED -1

void print_stack_item(const void *data)
{
    char *c = (char *)data;
    printf("%c\n", *c);
}

int is_opened_bracket(char c)
{
    return c == '(' || c == '[' || c == '{';
}
int is_closed_bracket(char c)
{
    return c == ')' || c == ']' || c == '}';
}
int is_bracket(char c)
{
    return is_opened_bracket(c) || is_closed_bracket(c);
}

int related_bracket(char bracket)
{
    if (is_closed_bracket(bracket))
    {
        switch (bracket)
        {
        case ')':
            return '(';
        case ']':
            return '[';
        case '}':
            return '{';
        default:
            break;
        }
    }
    else if (is_opened_bracket(bracket))
    {
        switch (bracket)
        {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        default:
            break;
        }
    }
    return '_';
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
        if (is_bracket(*cursor))
        {
            if (is_opened_bracket(*cursor))
            {
                stack_push(s, cursor);

                printf("LOG: Parentesi aperta: %c\n", *cursor);
                printf("LOG: Stack attuale:\n");
                stack_visit(s, print_stack_item);
            }
            else
            {
                char *last_opened = (char *)stack_pop(s);

                if (!last_opened || *last_opened != related_bracket(*cursor))
                {
                    printf("LOG: Stack vuoto non previsto oppure parentesi non corrispondente\n");
                    stack_free(s);
                    return position;
                }
                printf("LOG: Parentesi chiusa: %c - Ultima aperta: %c\n", *cursor, *last_opened);
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