#ifndef BTREE_H
#define BTREE_H
/*
    Definizione di albero binario
*/
typedef struct btree BTree;

/*
    Funzione per stabilire l'ordine degli elementi.
*/
typedef int (*compare_func)(void *, void *);
/*
    Funzione per la pulizia degli elementi interni all'albero.
*/
typedef void (*free_func)(void *);
/*
    Funzione che visita un singolo elemento dell'albero.
*/
typedef void (*visit_func)(void *);

/*
    Crea un albero binario.
*/
BTree *create_btree(compare_func comparer, free_func free, int allow_duplicates);
/*
    Legge la dimensione dell'albero.
*/
int btree_size(BTree *tree);
/*
    Aggiunge un elemento all'albero.
*/
int btree_append(BTree *tree, void *data);
/*
    Visita in PRE-ORDER.
*/
void pre_visit_btree(BTree *tree, visit_func visitor);
/*
    Visita in POST-ORDER.
*/
void post_visit_btree(BTree *tree, visit_func visitor);
/*
    Visita IN-ORDER.
*/
void visit_btree(BTree *tree, visit_func visitor);
/*
    Ricerca.
*/
void *btree_search(BTree *tree, void *target);
/*
    Liberazione della memoria.
*/
void destroy_btree(BTree *tree);
#endif