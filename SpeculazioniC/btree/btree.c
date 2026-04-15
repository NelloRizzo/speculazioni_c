#include "btree.h"

#include <stdlib.h>

struct node
{
    void *data;
    struct node *left;
    struct node *right;
};

struct btree
{
    compare_func comparer;
    free_func free;
    int allow_duplicates;
    struct node *head;
    int size;
};

BTree *create_btree(compare_func comparer, free_func free, int allow_duplicates)
{
    BTree *tree = malloc(sizeof(struct btree));
    if (!tree)
        return NULL;
    tree->allow_duplicates = allow_duplicates;
    tree->comparer = comparer;
    tree->free = free;
    tree->head = NULL;
    tree->size = 0;
    return tree;
}

int btree_size(BTree *tree)
{
    if (!tree)
        return 0;
    return tree->size;
}

void pre_visit_node(struct node *node, visit_func visitor)
{
    if (!node)
        return;
    visitor(node->data);
    pre_visit_node(node->left, visitor);
    pre_visit_node(node->right, visitor);
}

void pre_visit_btree(BTree *tree, visit_func visitor)
{
    if (!tree)
        return;
    pre_visit_node(tree->head, visitor);
}

void post_visit_node(struct node *node, visit_func visitor)
{
    if (!node)
        return;
    post_visit_node(node->left, visitor);
    post_visit_node(node->right, visitor);
    visitor(node->data);
}

void post_visit_btree(BTree *tree, visit_func visitor)
{
    if (!tree)
        return;
    post_visit_node(tree->head, visitor);
}

void visit_node(struct node *node, visit_func visitor)
{
    if (!node)
        return;
    visit_node(node->left, visitor);
    visitor(node->data);
    visit_node(node->right, visitor);
}

void visit_btree(BTree *tree, visit_func visitor)
{
    if (!tree)
        return;
    visit_node(tree->head, visitor);
}

void *search_node(compare_func comparer, struct node *node, void *data)
{
    if (!node)
        return NULL;

    int cmp = comparer(node->data, data);

    if (cmp == 0)
        return node->data; // Trovato!

    // Se il dato cercato è maggiore (cmp < 0), cerca a destra
    if (cmp < 0)
        return search_node(comparer, node->right, data);

    // Altrimenti cerca a sinistra
    return search_node(comparer, node->left, data);
}

void *btree_search(BTree *tree, void *data)
{
    if (!tree || !tree->head)
        return NULL;
    return search_node(tree->comparer, tree->head, data);
}

void clear_node(struct node *node, free_func free_data)
{
    if (!node)
        return;

    clear_node(node->left, free_data);
    clear_node(node->right, free_data);

    if (free_data)
        free_data(node->data); // Libera il contenuto
    free(node);                // Libera il nodo stesso
}

void destroy_btree(BTree *tree)
{
    if (!tree)
        return;
    clear_node(tree->head, tree->free);
    free(tree);
}

struct node *create_node(void *data)
{
    struct node *n = malloc(sizeof(struct node));
    if (!n)
        return NULL;
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

int append_node(compare_func comparer, struct node *node, void *data, int allow_duplicates)
{
    if (!node)
        return 0;
    int cmp = comparer(node->data, data);

    // L'uso del doppio puntatore **target permette di scrivere la logica di allocazione una volta sola per entrambi i rami
    struct node **target;

    if (cmp < 0) // maggiore a destra
        target = &(node->right);
    else if (cmp > 0 || allow_duplicates) // minore a sinistra
        target = &(node->left);
    else // se duplicato non ammesso
        return 0;

    if (*target)
        return append_node(comparer, *target, data, allow_duplicates);
    *target = create_node(data);
    return *target ? 1 : 0;
}

int btree_append(BTree *tree, void *data)
{
    if (!tree || !data)
        return 0;

    int res = 0;
    if (!tree->head)
    {
        tree->head = create_node(data);
        res = (tree->head != NULL);
    }
    else
    {
        res = append_node(tree->comparer, tree->head, data, tree->allow_duplicates);
    }
    tree->size += res;
    return res;
}
