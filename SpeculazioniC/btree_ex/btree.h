#ifndef BTREE_H
#define BTREE_H

#include "item.h"

typedef void (*visit_func)(Item);
typedef void (*free_func)(Item);

typedef struct node Node;

Node *create_node(Item data);
void node_set_left(Node *parent, Node *child);
void node_set_right(Node *parent, Node *child);
int tree_height(Node *root);
int count_nodes(Node *root);
void level_order(Node *root, visit_func visitor);
void pre_order_iter(Node *root, visit_func visitor);
void in_order_iter(Node *root, visit_func visitor);
void post_order_iter(Node *root, visit_func visitor);
void free_tree(Node *root, free_func free_item);

#endif
