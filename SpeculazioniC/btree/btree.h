#ifndef BTREE_H
#define BTREE_H

typedef struct btree BTree;

typedef int (*compare_func)(void *, void *);
typedef void (*free_func)(void *);
typedef void (*visit_func)(void *);

BTree *create_btree(compare_func comparer, free_func free, int allow_duplicates);
int btree_size(BTree *tree);
int btree_append(BTree *tree, void *data);
void pre_visit_btree(BTree *tree, visit_func visitor);
void post_visit_btree(BTree *tree, visit_func visitor);
void visit_btree(BTree *tree, visit_func visitor);
void *btree_search(BTree *tree, void *target);
void destroy_btree(BTree *tree);
#endif