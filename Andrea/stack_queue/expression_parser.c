#include "expression_parser.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

eval_ctx *eval_ctx_create(void)
{
    eval_ctx *ctx = malloc(sizeof(eval_ctx));
    if (!ctx)
        return NULL;
    ctx->chain = NULL;
    ctx->vars = NULL;
    return ctx;
}

expr_node *expr_number(double value)
{
    expr_node *node = malloc(sizeof(expr_node));
    if (!node)
        return NULL;
    node->type = NODE_NUMBER;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

expr_node *expr_var(const char *name)
{
    if (!name)
        return NULL;

    expr_node *node = malloc(sizeof(expr_node));
    if (!node)
        return NULL;

    node->type = NODE_VAR;
    node->left = NULL;
    node->right = NULL;
    strncpy(node->var, name, 31);
    node->var[31] = '\0';
    return node;
}

expr_node *expr_op(char op, expr_node *left, expr_node *right)
{
    expr_node *node = malloc(sizeof(expr_node));
    if (!node)
        return NULL;
    node->type = NODE_OP;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

void node_free(expr_node *node)
{
    free(node);
}

void expr_free(expr_node *node)
{
    if (!node)
        return;
    expr_free(node->left);
    expr_free(node->right);
    node_free(node);
}

static double handle_op(op_handler *h, char op, double left, double right, int *error)
{
    while (h)
    {
        if (h->op == op)
            return h->fn(left, right);
        h = h->next;
    }
    *error = ERROR_UNKNOWN_OPERATOR;
    return 0.0;
}

double expr_eval(const expr_node *node, const eval_ctx *ctx, int *error)
{
    if (!node || !ctx || !error)
        return 0.0;

    switch (node->type)
    {
    case NODE_NUMBER:
        return node->value;

    case NODE_VAR:
        return get_var_value(ctx->vars, node->var, error);

    case NODE_OP:
    {
        double left = expr_eval(node->left, ctx, error);
        if (*error)
            return 0.0;
        double right = expr_eval(node->right, ctx, error);
        if (*error)
            return 0.0;
        return handle_op(ctx->chain, node->op, left, right, error);
    }

    default:
        *error = ERROR_UNKNOWN_NODE;
        return 0.0;
    }
}

double get_var_value(const var_entry *vars, const char *var_name, int *error)
{
    if (!var_name || !error)
        return 0.0;

    const var_entry *entry = vars;
    while (entry)
    {
        if (strcmp(entry->name, var_name) == 0)
            return entry->value;
        entry = entry->next;
    }
    *error = ERROR_VAR_NOT_FOUND;
    return 0.0;
}

void eval_ctx_free(eval_ctx *ctx)
{
    if (!ctx)
        return;

    op_handler *h = ctx->chain;
    while (h)
    {
        op_handler *next = h->next;
        free(h);
        h = next;
    }

    var_entry *v = ctx->vars;
    while (v)
    {
        var_entry *next = v->next;
        free(v);
        v = next;
    }

    free(ctx);
}

void eval_ctx_set_var(eval_ctx *ctx, const char *name, double value)
{
    if (!ctx || !name)
        return;

    /* Aggiorna se la variabile esiste già */
    var_entry *entry = ctx->vars;
    while (entry)
    {
        if (strcmp(entry->name, name) == 0)
        {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    /* Altrimenti inserisce in testa */
    entry = malloc(sizeof(var_entry));
    if (!entry)
        return;

    strncpy(entry->name, name, 31);
    entry->name[31] = '\0';
    entry->value = value;
    entry->next = ctx->vars;
    ctx->vars = entry;
}

void eval_ctx_add_op(eval_ctx *ctx, char op, op_fn fn)
{
    if (!ctx || !fn)
        return;

    op_handler *handler = malloc(sizeof(op_handler));
    if (!handler)
        return;

    handler->op = op;
    handler->fn = fn; 
    handler->next = ctx->chain;
    ctx->chain = handler;
}