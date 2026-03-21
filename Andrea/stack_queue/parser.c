#include "parser.h"
#include <string.h>
#include <stdlib.h>

/* Avanza al prossimo token e restituisce quello consumato.
   Il chiamante è responsabile di liberarlo con free(). */
static token *parser_advance(parser *p)
{
    token *old = p->current;
    p->current = lexer_next(p->l);
    return old;
}

/* ─── factor → NUMBER | VAR | '(' expr ')' ──────────────────────────────── */
expr_node *parse_factor(parser *p)
{
    if (!p || p->error)
        return NULL;

    token *t = p->current;
    if (!t)
    {
        p->error = ERROR_UNEXPECTED_TOKEN;
        return NULL;
    }

    /* ── numero ── */
    if (t->type == TOKEN_NUMBER)
    {
        double val = t->value;
        free(parser_advance(p)); /* consuma e libera il token */
        return expr_number(val);
    }

    /* ── variabile ── */
    if (t->type == TOKEN_VAR)
    {
        char name[32];
        strncpy(name, t->var, 31);
        name[31] = '\0';
        free(parser_advance(p));
        return expr_var(name);
    }

    /* ── espressione parentesizzata ── */
    if (t->type == TOKEN_LPAREN)
    {
        free(parser_advance(p)); /* consuma '(' */

        expr_node *inner = parse_expr(p);
        if (!inner || p->error)
        {
            expr_free(inner);
            return NULL;
        }

        /* ci aspettiamo ')' */
        if (!p->current || p->current->type != TOKEN_RPAREN)
        {
            p->error = ERROR_UNMATCHED_PAREN;
            expr_free(inner);
            return NULL;
        }
        free(parser_advance(p)); /* consuma ')' */
        return inner;
    }

    p->error = ERROR_UNEXPECTED_TOKEN;
    return NULL;
}

/* ─── term → factor { ('*' | '/') factor } ──────────────────────────────── */
expr_node *parse_term(parser *p)
{
    if (!p || p->error)
        return NULL;

    expr_node *node = parse_factor(p);
    if (!node || p->error)
        return node;

    while (p->current &&
           p->current->type == TOKEN_OP &&
           (p->current->op == '*' || p->current->op == '/'))
    {
        char op = p->current->op;
        free(parser_advance(p)); /* consuma l'operatore */

        expr_node *right = parse_factor(p);
        if (!right || p->error)
        {
            expr_free(node);
            expr_free(right);
            return NULL;
        }
        node = expr_op(op, node, right);
        if (!node) /* malloc fallita */
        {
            p->error = ERROR_UNKNOWN_NODE;
            return NULL;
        }
    }
    return node;
}

/* ─── expr → term { ('+' | '-') term } ──────────────────────────────────── */
expr_node *parse_expr(parser *p)
{
    if (!p || p->error)
        return NULL;

    expr_node *node = parse_term(p);
    if (!node || p->error)
        return node;

    while (p->current &&
           p->current->type == TOKEN_OP &&
           (p->current->op == '+' || p->current->op == '-'))
    {
        char op = p->current->op;
        free(parser_advance(p));

        expr_node *right = parse_term(p);
        if (!right || p->error)
        {
            expr_free(node);
            expr_free(right);
            return NULL;
        }
        node = expr_op(op, node, right);
        if (!node)
        {
            p->error = ERROR_UNKNOWN_NODE;
            return NULL;
        }
    }
    return node;
}

/* ─── Ciclo di vita ──────────────────────────────────────────────────────── */

parser *parser_create(lexer *l)
{
    if (!l)
        return NULL;

    parser *p = malloc(sizeof(parser));
    if (!p)
        return NULL;

    p->l = l;
    p->error = 0;
    p->current = lexer_next(l); /* carica il primo token di lookahead */
    return p;
}

void parser_free(parser *p)
{
    if (!p)
        return;
    /* libera l'eventuale token di lookahead rimasto */
    free(p->current);
    free(p);
    /* nota: il lexer è owned dal chiamante, non viene liberato qui */
}