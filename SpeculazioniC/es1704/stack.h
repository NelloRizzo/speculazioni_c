#ifndef STACK_H
#define STACK_H
/* Definizione di stack adt */
typedef struct stack Stack;
/* Funzione per visitare lo stack senza consumare gli elementi */
typedef void (*visit_fn)(const void *);
/* Crea lo stack */
Stack *stack_create(void);
/* Osserva senza cancellare l'elemento in testa allo stack */
void *stack_peek(Stack *s);
/* Preleva e restituisce l'ultimo elemento inserito */
void *stack_pop(Stack *s);
/* Inserisce un elemento in stack */
void stack_push(Stack *s, void *data);
/* Restituisce le dimensioni dello stack */
int stack_size(Stack *s);
/* Restituisce 1 se lo stack è vuoto */
int stack_empty(Stack *s);
/* Libera la memoria dello stack (ATTENZIONE: NON DISTRUGGE GLI ELEMENTI GESTITI) */
void stack_free(Stack *s);
/* Visita lo stack */
void stack_visit(Stack *s, visit_fn visitor);

#endif