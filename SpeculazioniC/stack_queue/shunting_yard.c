#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shunting_yard.h"
#include "stack.h"

int get_precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

queue *shunting_yard(lexer *l)
{
    stack *op_stack = stack_create();
    queue *output_queue = queue_create();
    token *t;

    while ((t = lexer_next(l)) != NULL)
    {
        if (t->type == TOKEN_END)
        {
            free(t); // il token END ha esaurito il suo compito
            break;
        }

        if (t->type == TOKEN_NUMBER)
        {
            // Regola: Numeri vanno direttamente in coda
            queue_enqueue(output_queue, t);
        }

        else if (t->type == TOKEN_VAR)
        {
            // Peek per vedere se il prossimo token è una '('
            // Se sì, è una funzione. Altrimenti è una variabile.
            // (In alternativa, puoi semplicemente trattare tutte le variabili
            // nello stack se hanno un nome conosciuto come "sin")
            char c = lexer_peek_char(l);
            // REGOLA: Se è una funzione, spingila nello STACK degli operatori
            if (c == '(')
            {
                t->type = TOKEN_FUNC; // Cambiamo il tipo per chiarezza
                stack_push(op_stack, t);
            }
            else
            {
                queue_enqueue(output_queue, t); // Variabile normale
            }
        }

        else if (t->type == TOKEN_LPAREN)
        {
            // Regola: Parentesi aperta va nello stack
            stack_push(op_stack, t);
        }

        else if (t->type == TOKEN_RPAREN)
        {
            // Regola: Svuota lo stack nella coda fino alla parentesi aperta
            while (!stack_empty(op_stack) && ((token *)op_stack->head->content)->type != TOKEN_LPAREN)
            {
                queue_enqueue(output_queue, stack_pop(op_stack));
            }
            free(stack_pop(op_stack)); // Rimuovi '('

            // Se dopo la parentesi c'è una funzione in cima allo stack, spostala in coda
            if (!stack_empty(op_stack))
            {
                token *top = (token *)op_stack->head->content;
                if (top->type == TOKEN_FUNC)
                {
                    queue_enqueue(output_queue, stack_pop(op_stack));
                }
            }
            free(t); // Libera ')'
        }

        else if (t->type == TOKEN_OP)
        {
            // Regola: Gestione precedenza degli operatori
            while (!stack_empty(op_stack))
            {
                token *top_token = (token *)op_stack->head->content;
                if (top_token->type == TOKEN_OP &&
                    get_precedence(top_token->op) >= get_precedence(t->op))
                {
                    queue_enqueue(output_queue, stack_pop(op_stack));
                }
                else
                {
                    break;
                }
            }
            stack_push(op_stack, t);
        }

        else if (t->type == TOKEN_ERROR)
        {
            // Gestione errore (si potrebbe restituire NULL o gestire diversamente)
            fprintf(stderr, "Errore lessicale ignorato\n");
            free(t);
        }
    }

    // Svuota lo stack rimanente nella coda
    while (!stack_empty(op_stack))
    {
        token *remaining = stack_pop(op_stack);
        if (remaining->type == TOKEN_LPAREN || remaining->type == TOKEN_RPAREN)
        {
            // Parentesi appese: errore di sintassi
            free(remaining);
        }
        else
        {
            queue_enqueue(output_queue, remaining);
        }
    }

    // Pulizia finale dello stack (ma non del contenuto, che ora è in coda)
    stack_free(op_stack, NULL);

    return output_queue;
}

double evaluate_rpn(queue *q, const symbol_table *st, const fn_cor *functions)
{
    stack *s = stack_create();
    token *t;
    queue_node *current = q->head;

    while (current != NULL)
    {
        t = (token *)current->content;

        if (t->type == TOKEN_NUMBER || t->type == TOKEN_VAR)
        {
            double *val = malloc(sizeof(double));

            if (t->type == TOKEN_NUMBER)
            {
                *val = t->value;
            }
            else
            {
                // È una variabile: cerca il valore nella tabella
                *val = get_variable_value(st, t->var);
            }

            stack_push(s, val);
        }
        else if (t->type == TOKEN_OP)
        {
            // Estraggo i due operandi
            double *ptr_b = (double *)stack_pop(s);
            double *ptr_a = (double *)stack_pop(s);

            if (!ptr_a || !ptr_b)
            {
                fprintf(stderr, "Errore: Operandi insufficienti per l'operatore\n");
                if (ptr_a)
                    free(ptr_a);
                if (ptr_b)
                    free(ptr_b);
                return 0.0; // O valutare se gestire l'errore diversamente
            }
            double b = *ptr_b;
            double a = *ptr_a;
            free(ptr_b);
            free(ptr_a);

            double res = 0;
            switch (t->op)
            {
            case '+':
                res = a + b;
                break;
            case '-':
                res = a - b;
                break;
            case '*':
                res = a * b;
                break;
            case '/':
                res = a / b;
                break;
            }

            // Metto il risultato nello stack
            double *val_res = malloc(sizeof(double));
            *val_res = res;
            stack_push(s, val_res);
        }
        else if (t->type == TOKEN_FUNC)
        {
            double *ptr_arg = (double *)stack_pop(s);
            if (!ptr_arg)
            {
                fprintf(stderr, "Errore: argomenti insufficienti per %s\n", t->var);
                return 0.0;
            }

            double arg = *ptr_arg;
            free(ptr_arg);

            // Chiamata alla CoR
            double res = execute(functions, t->var, arg);

            double *val_res = malloc(sizeof(double));
            *val_res = res;
            stack_push(s, val_res);
        }
        current = current->next;
    }

    // Il risultato finale è l'ultimo elemento nello stack
    double *final_ptr = (double *)stack_pop(s);
    double result = *final_ptr;

    free(final_ptr);
    stack_free(s, free); // Pulizia dello stack

    return result;
}

// Funzione per cercare il valore di una variabile
double get_variable_value(const symbol_table *st, const char *name)
{
    for (size_t i = 0; i < st->count; i++)
    {
        if (strcmp(st->entries[i].name, name) == 0)
        {
            return st->entries[i].value;
        }
    }
    fprintf(stderr, "Errore: Variabile '%s' non definita. Uso 0.0\n", name);
    return 0.0;
}

fn_cor *create_func_chain(void)
{
    fn_cor *cor = malloc(sizeof(fn_cor));
    if (!cor)
        return NULL;
    cor->head = NULL;
    cor->count = 0;
    return cor;
}

void add_handler(fn_cor *chain, const char *func_name, func_ptr func)
{
    if (!chain)
        return;

    func_handler *handler = malloc(sizeof(func_handler));
    if (!handler)
        return;
    handler->executor = func;
    strncpy(handler->name, func_name, sizeof(handler->name) - 1);
    handler->name[sizeof(handler->name) - 1] = '\0';
    handler->next = chain->head;
    chain->head = handler;
    chain->count++;
}

void remove_handler(fn_cor *chain, const char *name)
{
    if (!chain || !chain->head || !name)
        return;

    func_handler *current = chain->head;
    func_handler *prev = NULL;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            // Se è il primo della lista
            if (prev == NULL)
                chain->head = current->next;
            else
                prev->next = current->next;

            free(current);
            chain->count--;
            return; // Trovato e rimosso
        }
        prev = current;
        current = current->next;
    }
}

double execute(const fn_cor *chain, const char *func_name, double argument)
{
    func_handler *cursor = chain->head;
    while (cursor)
    {
        if (strcmp(func_name, cursor->name) == 0)
            return cursor->executor(argument);
        cursor = cursor->next;
    }
    fprintf(stderr, "Errore: funzione '%s' non definita. Uso 0.0\n", func_name);
    return 0.0;
}
void free_func_chain(fn_cor *chain)
{
    if (!chain)
        return;
    func_handler *current = chain->head;
    while (current)
    {
        func_handler *next = current->next;
        free(current);
        current = next;
    }
    free(chain);
}