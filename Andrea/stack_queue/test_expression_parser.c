#include <stdio.h>
#include "expression_parser.h"
#include "parser.h"
#include "lexer.h"

/* ─── Operatori ─────────────────────────────────────────────────────────── */
static double op_add(double l, double r) { return l + r; }
static double op_sub(double l, double r) { return l - r; }
static double op_mul(double l, double r) { return l * r; }
static double op_div(double l, double r) { return l / r; }

/* Registra tutti gli operatori nel contesto */
static void register_ops(eval_ctx *ctx)
{
    eval_ctx_add_op(ctx, '+', op_add);
    eval_ctx_add_op(ctx, '-', op_sub);
    eval_ctx_add_op(ctx, '*', op_mul);
    eval_ctx_add_op(ctx, '/', op_div);
}

/* Valuta una stringa e stampa il risultato */
static void eval_string(const char *src, eval_ctx *ctx)
{
    lexer *lex = lexer_create(src);
    parser *p = parser_create(lex);

    if (!lex || !p)
    {
        fprintf(stderr, "Errore: allocazione fallita\n");
        lexer_free(lex);
        parser_free(p);
        return;
    }

    expr_node *tree = parse_expr(p);

    if (p->error || !tree)
    {
        fprintf(stderr, "Errore di parsing (codice %d) su: \"%s\"\n", p->error, src);
    }
    else
    {
        int error = 0;
        double result = expr_eval(tree, ctx, &error);
        if (error)
            fprintf(stderr, "Errore di valutazione (codice %d) su: \"%s\"\n", error, src);
        else
            printf("  %s  =  %g\n", src, result);
    }

    expr_free(tree);
    parser_free(p);
    lexer_free(lex);
}

void parser_full_integration_test(void)
{
    /* ── 1. Test con albero costruito a mano ─────────────────────────────── */
    printf("=== Test albero diretto ===\n");

    eval_ctx *ctx = eval_ctx_create();
    register_ops(ctx);

    /* (3 + 5) */
    expr_node *tree = expr_op('+', expr_number(3), expr_number(5));
    int error = 0;
    double result = expr_eval(tree, ctx, &error);
    if (!error)
        printf("  3 + 5  =  %g\n", result);
    expr_free(tree);

    /* variabile x = 10; x * 2 */
    eval_ctx_set_var(ctx, "x", 10.0);
    tree = expr_op('*', expr_var("x"), expr_number(2));
    error = 0;
    result = expr_eval(tree, ctx, &error);
    if (!error)
        printf("  x * 2  =  %g  (x=10)\n", result);
    expr_free(tree);

    /* ── 2. Test con parser ──────────────────────────────────────────────── */
    printf("\n=== Test parser ===\n");
    eval_string("3 + 2 - 4", ctx);
    eval_string("2 * 3 + 4", ctx);
    eval_string("2 * (3 + 4)", ctx);
    eval_string("10 / 2 + 1", ctx);
    eval_string("x * 2 + 1", ctx); /* x=10 → 21 */
    eval_string("(1 + 2) * (3 - 1)", ctx);

    eval_ctx_free(ctx);
}
