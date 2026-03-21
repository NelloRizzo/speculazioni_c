#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "strings.h"

void free_find_result(find_result *p)
{
    if (!p)
        return;
    if (p->target)
        free(p->target);
    free(p);
}

size_t char_array_len(const char *s)
{
    if (!s)
        return 0;
    const char *p = s;
    while (*p)
        p++;
    return (size_t)(p - s);
}

size_t string_len(const string *s)
{
    size_t len = 0;
    while (s)
    {
        len += s->len;
        s = s->next;
    }
    return len;
}

string *string_tail(string *node)
{
    if (!node)
        return NULL;
    while (node->next)
    {
        node = node->next;
    }
    return node;
}

string *string_head(string *node)
{
    if (!node)
        return NULL;
    while (node->prev)
    {
        node = node->prev;
    }
    return node;
}

string *string_create(const char *s)
{
    if (!s)
        return NULL;

    return string_append(s, NULL);
}

void str_cpy(char *dest, const char *source, size_t len)
{
    if (!source || !dest)
        return;

    dest[len] = '\0';
    while (len--)
    {
        dest[len] = source[len];
    }
}
string *string_append(const char *s, string *parent)
{
    if (!s)
        return parent;

    size_t len = char_array_len(s);
    string *str = malloc(sizeof(string));
    if (!str)
        return parent;

    str->content = malloc(len + 1);
    if (!str->content)
    {
        free(str);
        return parent;
    }
    str->len = len;
    str_cpy(str->content, s, len);
    str->next = NULL;
    str->prev = NULL;
    if (!parent)
        return str;
    string *link = string_tail(parent);
    str->prev = link;
    link->next = str;
    return string_head(parent);
}

string *string_insert(const char *s, string *parent)
{
    if (!s || !parent)
        return parent;

    string *node = string_create(s);
    if (!node)
        return parent;
    if (parent->prev)
    {
        node->prev = parent->prev;
        parent->prev->next = node;
    }
    node->next = parent;
    parent->prev = node;
    return string_head(node);
}

string *string_append_node(string *node, string *parent)
{
    if (!node || !parent)
        return parent;

    string *link = string_tail(parent);
    node->prev = link;
    link->next = node;
    return parent;
}

string *string_clone_node(string *node)
{
    string *clone = malloc(sizeof(string));
    if (!clone)
        return NULL;
    clone->content = malloc(node->len + 1);
    if (!clone->content)
    {
        free(clone);
        return NULL;
    }
    clone->len = node->len;
    str_cpy(clone->content, node->content, node->len);
    clone->next = NULL;
    clone->prev = NULL;
    return clone;
}

string *string_remove_node(string *node)
{
    if (!node)
        return NULL;

    string *neighbour = node->prev ? node->prev : node->next;

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;

    free(node->content);
    free(node);
    return string_head(neighbour);
}

string *string_prepend(const char *s, string *child)
{
    if (!s)
        return NULL;

    string *p = string_create(s);
    if (!p)
        return NULL;

    if (child)
    {
        p->next = child;
        p->prev = child->prev;
        if (child->prev)
            child->prev->next = p;
        child->prev = p;
    }
    string *head = p;
    while (head->prev)
    {
        head = head->prev;
    }

    return head;
}

void string_free(string *s)
{
    while (s)
    {
        string *next = s->next;
        free(s->content);
        free(s);
        s = next;
    }
}

void visit(string *s, string_func apply)
{
    if (!apply)
        return;
    while (s)
    {
        apply(s);
        s = s->next;
    }
}

int string_all(const string *s, string_predicate predicate)
{
    if (!predicate)
        return -1;

    while (s)
    {
        if (!predicate(s))
            return 0;
        s = s->next;
    }
    return 1;
}

int string_any(const string *s, string_predicate predicate)
{
    if (!predicate)
        return -1;

    while (s)
    {
        if (predicate(s))
            return 1;
        s = s->next;
    }
    return 0;
}

void string_fold(const string *s, string_reducer reduce, void *accumulator)
{
    if (!reduce)
        return;

    while (s)
    {
        reduce(s, accumulator);
        s = s->next;
    }
}

string *string_split(string *s, int pos)
{
    if (!s || pos < 0)
        return NULL;
    if (pos == 0)
        return s;
    size_t p = (size_t)pos;
    char *content = s->content;
    size_t len = s->len;
    char *left = malloc(p + 1);
    if (!left)
        return NULL;
    char *right = malloc(len - p + 1);
    if (!right)
        return NULL;
    str_cpy(left, content, p);
    str_cpy(right, content + p, len - p);
    free(s->content);
    s->content = malloc(p + 1);
    if (!s->content)
    {
        free(left);
        free(right);
        return NULL;
    }
    str_cpy(s->content, left, p);
    free(left);

    string *node = string_create(right);
    free(right);
    if (!node)
        return NULL;
    node->next = s->next;
    node->prev = s;
    if (s->next)
        s->next->prev = node;
    s->next = node;
    return s;
}

// Usato per la riduzione della lista in una stringa in stile C
struct accumulator
{
    // la stringa attuale da concatenare
    char *str;
    // il punto da cui cominciare a concatenare
    size_t start;
};

// Riduzione di una stringa nell'accumulatore
void to_string(const string *s, void *acc)
{
    if (!s || !acc)
        return;
    struct accumulator *a = acc;
    // copia la stringa attuale nell'accumulatore a partire da 'start'
    str_cpy(a->str + a->start, s->content, s->len);
    // incrementa 'start' dell'accumulatore per consentire il successivo concatenamento
    a->start += s->len;
}

char *string_to_char_array(const string *s)
{
    struct accumulator acc;
    size_t len = string_len(s);
    acc.str = malloc(len + 1);
    if (!acc.str)
        return NULL;

    acc.start = 0;
    string_fold(s, to_string, &acc);
    // garantisce una stringa valida nel caso di len == 0
    acc.str[len] = '\0';
    return acc.str;
}

int find_in_char_array(const char *text, const char *target)
{
    if (!text || !target)
        return -1;

    int pos = -1;
    const char *p = text;
    while (*p && pos == -1)
    {
        if (*p == *target)
        {
            size_t t_len = char_array_len(target);
            size_t c = 0;
            while (c < t_len && *(p + c) && target[c] == *(p + c))
                c++;
            if (c == t_len)
                pos = (int)(p - text);
        }
        p++;
    }
    return pos;
}

find_result *string_find(string *s, const char *target)
{
    if (!s || !target)
        return NULL;

    find_result *result = malloc(sizeof(find_result));
    if (!result)
        return NULL;

    result->found = NULL;
    size_t t_len = char_array_len(target);
    result->target = malloc(t_len + 1);
    if (!result->target)
    {
        free_find_result(result);
        return NULL;
    }
    str_cpy(result->target, target, t_len);
    int pos = -1;
    while (s && pos == -1)
    {
        pos = find_in_char_array(s->content, target);
        if (pos == -1)
            s = s->next;
    }
    if (pos > -1)
    {
        result->found = s;
        result->position = pos;
    }
    return result;
}
