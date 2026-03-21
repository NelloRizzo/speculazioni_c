#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "lexer.h"
#include "queue.h"

typedef struct
{
    char name[32];
    double value;
} symbol;

typedef struct
{
    symbol *entries;
    size_t count;
} symbol_table;

// Firma della funzione di calcolo
typedef double (*func_ptr)(double);

typedef struct func_handler
{
    char name[16];             // Nome della funzione (es. "sin")
    func_ptr executor;         // Puntatore alla funzione
    struct func_handler *next; // Prossimo anello
} func_handler;

typedef struct
{
    func_handler *head;
    size_t count;
} fn_cor;

queue *shunting_yard(lexer *l);
double evaluate_rpn(queue *q, const symbol_table *st, const fn_cor* functions);
double get_variable_value(const symbol_table *st, const char *name);

fn_cor *create_func_chain(void);
void add_handler(fn_cor *chain, const char *func_name, func_ptr func);
void remove_handler(fn_cor *chain, const char *name);
void free_func_chain(fn_cor *chain);

double execute(const fn_cor *chain, const char *func_name, double argument);

#endif