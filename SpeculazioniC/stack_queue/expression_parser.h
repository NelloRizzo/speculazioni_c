#ifndef EXPR_H
#define EXPR_H

#include <stddef.h>

/* ─── Nodo dell'albero ───────────────────────────────────────────────────── */

typedef enum
{
    NODE_NUMBER, /* foglia: valore numerico */
    NODE_VAR,    /* foglia: variabile (es. "x") */
    NODE_OP,     /* nodo interno: operatore */
} node_type;

typedef struct expr_node
{
    node_type type;
    union
    {
        double value; /* NODE_NUMBER */
        char var[32]; /* NODE_VAR    */
        char op;      /* NODE_OP     */
    };
    struct expr_node *left;
    struct expr_node *right;
} expr_node;

/* ─── Chain of Responsibility per gli operatori ──────────────────────────── */

/* puntatore a funzione per operatori binari */
typedef double (*op_fn)(double left, double right);

typedef struct op_handler
{
    char op;  /* operatore gestito (es. '+') */
    op_fn fn; /* funzione di valutazione     */
    struct op_handler *next;
} op_handler;

/* ─── Contesto di valutazione ────────────────────────────────────────────── */

typedef struct var_entry
{
    char name[32];
    double value;
    struct var_entry *next;
} var_entry;

typedef struct
{
    var_entry *vars;   /* lista di variabili  */
    op_handler *chain; /* catena degli operatori */
} eval_ctx;

typedef enum
{
    ERROR_NONE = 0,
    ERROR_VAR_NOT_FOUND,
    ERROR_UNKNOWN_NODE,
    ERROR_UNKNOWN_OPERATOR,
    ERROR_UNEXPECTED_TOKEN,
    ERROR_UNMATCHED_PAREN,
} expr_error;

/* ─── API ─────────────────────────────────────────────────────────────────── */

expr_node *expr_number(double value);
expr_node *expr_var(const char *name);
expr_node *expr_op(char op, expr_node *left, expr_node *right);

void expr_free(expr_node *node);
void node_free(expr_node *node);

double expr_eval(const expr_node *node, const eval_ctx *ctx, int *error);
double get_var_value(const var_entry *vars, const char *var_name, int *error);

eval_ctx *eval_ctx_create(void);
void eval_ctx_free(eval_ctx *ctx);
void eval_ctx_set_var(eval_ctx *ctx, const char *name, double value);
void eval_ctx_add_op(eval_ctx *ctx, char op, op_fn fn);

#endif /* EXPR_H */