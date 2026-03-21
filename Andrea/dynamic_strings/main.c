#include <stdio.h>
#include <stdlib.h>
#include "strings.h"

void my_print(const string *s)
{
    printf("%s", s->content);
}

void count_vowels(const string *s, void *count) // <- QUI *count DEVE essere *int (v. definizione di string_fold e string_reducer)
{
    if (!s || !count)
        return;

    int *c = (int *)count;
    const char *p = s->content;
    while (*p)
    {
        *c += (*p == 'a' || *p == 'e' || *p == 'i' || *p == 'o' || *p == 'u' || *p == 'A' || *p == 'E' || *p == 'I' || *p == 'O' || *p == 'U');
        p++;
    }
}

int main(void)
{
    // Inizia la lista
    string *s = string_create("Ciao");
    // aggiunge una stringa
    s = string_append(" a tutti!", s);
    // crea un nuovo nodo
    string *p = string_create(" Come va?");
    // e lo aggiunge alla fine
    s = string_append_node(p, s);
    // aggiuge una nuova stringa prima della precedente
    s = string_prepend(" State bene?", p);
    printf("Stampa della lista attraverso il metodo visit():\n");
    visit(s, my_print);
    printf("\n");
    size_t l = string_len(s);
    printf("Lunghezza: %zu\n", l);
    int v = 0;
    string_fold(s, count_vowels, &v);
    printf("Totale vocali calcolato con string_fold(): %d\n", v);

    char *text = string_to_char_array(s);
    printf("Lista come char*: \"%s\"\n", text);
    free(text);
    find_result *r = string_find(s, "bene");
    if (r)
        if (r->found)
        {
            printf("La stringa cercata è stata trovata nel nodo contenente \"%s\" in posizione %d\n", r->found->content, r->position);

            string_split(r->found, r->position);

            s = string_insert("tutti ", r->found->next);
            printf("Inserimento:\n");
            visit(s, my_print);
            printf("\n");

            free_find_result(r);
        }
    string *c = string_clone_node(s);
    s = string_append(" ", s);
    s = string_append_node(c, s);
    s = string_append("!", s);
    visit(s, my_print);
    printf("\n");
    string_free(s);
}