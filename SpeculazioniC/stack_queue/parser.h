#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "expression_parser.h"

typedef struct
{
    lexer *l;       /* lexer sottostante  */
    token *current; /* token di lookahead */
    int error;      /* codice errore      */
} parser;

parser *parser_create(lexer *l);
void parser_free(parser *p);

/* Grammatica (recursive descent, precedenza crescente):
 *
 *   expr   → term   { ('+' | '-') term   }
 *   term   → factor { ('*' | '/') factor }
 *   factor → NUMBER | VAR | '(' expr ')'
 */
expr_node *parse_expr(parser *p);
expr_node *parse_term(parser *p);
expr_node *parse_factor(parser *p);

#endif /* PARSER_H */