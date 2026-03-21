#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stddef.h>

typedef enum
{
    TOKEN_NUMBER, /* 3.14 */
    TOKEN_VAR,    /* x, y, foo */
    TOKEN_OP,     /* + - * / */
    TOKEN_LPAREN, /* ( */
    TOKEN_RPAREN, /* ) */
    TOKEN_FUNC,   /* funzione */
    TOKEN_END,    /* fine della stringa */
    TOKEN_ERROR,  /* carattere non riconosciuto */
} token_type;

typedef struct
{
    token_type type;
    union
    {
        double value; /* TOKEN_NUMBER */
        char var[32]; /* TOKEN_VAR    */
        char op;      /* TOKEN_OP     */
    };
} token;

typedef struct
{
    char *src;  /* copia della stringa (owned) */
    size_t pos; /* posizione corrente          */
} lexer;

lexer *lexer_create(const char *src);
token *lexer_next(lexer *l);
void lexer_free(lexer *l);
char lexer_peek_char(lexer *l);

#endif /* LEXER_H */