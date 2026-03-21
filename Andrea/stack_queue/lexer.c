#include <string.h>
#include "lexer.h"

static int is_digit(char c) { return c >= '0' && c <= '9'; }
static int is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
static int is_alnum(char c) { return is_alpha(c) || is_digit(c); }
static int is_decimal_sep(char c) { return c == '.' || c == ','; }
static int is_operator(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

/* Legge un numero (intero o decimale) dalla posizione corrente del lexer */
static token *token_number(lexer *l)
{
    token *t = malloc(sizeof(token));
    if (!t)
        return NULL;

    double integral = 0.0;
    double decimal = 0.1;
    int in_decimal = 0;

    while (l->src[l->pos] && (is_digit(l->src[l->pos]) || is_decimal_sep(l->src[l->pos])))
    {
        char c = l->src[l->pos];
        if (is_digit(c))
        {
            if (in_decimal)
            {
                integral += (c - '0') * decimal; /* FIX: era (c - 0) */
                decimal *= 0.1;
            }
            else
            {
                integral = integral * 10.0 + (c - '0');
            }
        }
        else /* separatore decimale */
        {
            if (in_decimal) /* secondo separatore: token malformato */
            {
                free(t);
                return NULL;
            }
            in_decimal = 1;
        }
        l->pos++;
    }

    t->type = TOKEN_NUMBER;
    t->value = integral;
    return t;
}

/* Crea un lexer copiando la stringa sorgente */
lexer *lexer_create(const char *src)
{
    if (!src)
        return NULL;

    lexer *l = malloc(sizeof(lexer));
    if (!l)
        return NULL;

    size_t len = strlen(src);
    l->src = malloc(len + 1);
    if (!l->src)
    {
        free(l);
        return NULL;
    }
    memcpy(l->src, src, len);
    l->src[len] = '\0'; /* terminatore */
    l->pos = 0;
    return l;
}

/* Libera il lexer e la stringa sorgente */
void lexer_free(lexer *l)
{
    if (!l)
        return;
    free(l->src);
    free(l);
}

/* Restituisce il prossimo token; il chiamante deve liberarlo con free() */
token *lexer_next(lexer *l)
{
    if (!l)
        return NULL;

    /* Salta spazi e tabulazioni */
    while (l->src[l->pos] == ' ' || l->src[l->pos] == '\t')
        l->pos++;

    char c = l->src[l->pos];
    token *t = NULL;

    if (c == '\0')
    {
        t = malloc(sizeof(token));
        if (t)
            t->type = TOKEN_END;
    }
    else if (is_digit(c) || is_decimal_sep(c))
    {
        t = token_number(l);
    }
    else if (is_operator(c))
    {
        t = malloc(sizeof(token));
        if (t)
        {
            t->type = TOKEN_OP;
            t->op = c;
            l->pos++;
        }
    }
    else if (c == '(' || c == ')')
    {
        t = malloc(sizeof(token));
        if (t)
        {
            t->type = (c == '(') ? TOKEN_LPAREN : TOKEN_RPAREN;
            l->pos++;
        }
    }
    else if (is_alpha(c))
    {
        t = malloc(sizeof(token));
        if (t)
        {
            size_t i = 0;
            while (l->src[l->pos] && is_alnum(l->src[l->pos]) && i < 31)
                t->var[i++] = l->src[l->pos++];
            t->var[i] = '\0';
            t->type = TOKEN_VAR;
        }
    }

    if (!t)
    {
        /* Carattere non riconosciuto: avanza e segnala errore */
        t = malloc(sizeof(token));
        if (t)
            t->type = TOKEN_ERROR;
        l->pos++;
    }
    return t;
}

char lexer_peek_char(lexer *l) {
    size_t temp_pos = l->pos;
    // Salta temporaneamente gli spazi per vedere il prossimo carattere utile
    while (l->src[temp_pos] == ' ' || l->src[temp_pos] == '\t') {
        temp_pos++;
    }
    return l->src[temp_pos];
}