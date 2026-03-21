#include <stdio.h>

#include "lexer.h"
#include "shunting_yard.h"

#include <math.h>

double sqr(double x) { return x * x; }

void sy_full_integration_test(void)
{
    // 1. Setup Funzioni (CoR)
    fn_cor *functions = create_func_chain();
    add_handler(functions, "sin", sin);
    add_handler(functions, "cos", cos);
    add_handler(functions, "sqrt", sqrt);
    add_handler(functions, "sqr", sqr);

    // 2. Setup Variabili (Symbol Table)
    symbol vars[] = {{"x", 0.0}, {"pi", 3.14159265}};
    symbol_table st = {vars, 2};

    // 3. Esecuzione: sin(pi / 2)
    char *expr = "sqr(x) + 2*x + 1";
    lexer *l = lexer_create(expr);
    queue *q = shunting_yard(l);

    if (q)
    {
        for (double i = 0.0; i < 10; i++)
        {
            st.entries[0].value = i;
            double res = evaluate_rpn(q, &st, functions);
            printf("x: %.4f %s: %.4f\n", i, expr, res);
        }

        queue_free(q, free);
    }

    // 4. Cleanup
    lexer_free(l);
    free_func_chain(functions);
}

int main(void)
{
    sy_full_integration_test();
    return 0;
}