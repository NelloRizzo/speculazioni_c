#include <stdlib.h>
#include "btree.h"
#include "stack.h"
#include "queue.h"

struct node {
    Item data;
    struct node *left;
    struct node *right;
};

Node *create_node(Item data)
{
    Node *n = malloc(sizeof(Node));
    if (!n)
        return NULL;
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void node_set_left(Node *parent, Node *child)
{
    if (parent)
        parent->left = child;
}

void node_set_right(Node *parent, Node *child)
{
    if (parent)
        parent->right = child;
}

int tree_height(Node *root)
{
    if (!root)
        return 0;
    int left = tree_height(root->left);
    int right = tree_height(root->right);
    return (left > right ? left : right) + 1;
}

int count_nodes(Node *root)
{
    if (!root)
        return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

void level_order(Node *root, visit_func visitor)
{
    if (!root)
        return;
    Queue *q = create_queue();
    if (!q)
        return;
    enqueue(q, (Item)root);
    while (!is_empty_q(q))
    {
        Node *curr = (Node *)dequeue(q);
        visitor(curr->data);
        if (curr->left)
            enqueue(q, (Item)curr->left);
        if (curr->right)
            enqueue(q, (Item)curr->right);
    }
    free_queue(q);
}

void pre_order_iter(Node *root, visit_func visitor)
{
    if (!root)
        return;
    Stack *s = create_stack();
    if (!s)
        return;
    push(s, (Item)root);
    while (!is_empty(s))
    {
        Node *curr = (Node *)pop(s);
        visitor(curr->data);
        if (curr->right)
            push(s, (Item)curr->right);
        if (curr->left)
            push(s, (Item)curr->left);
    }
    free_stack(s);
}

void in_order_iter(Node *root, visit_func visitor)
{
    Stack *s = create_stack();
    if (!s)
        return;
    Node *curr = root;
    while (curr || !is_empty(s))
    {
        while (curr)
        {
            push(s, (Item)curr);
            curr = curr->left;
        }
        curr = (Node *)pop(s);
        visitor(curr->data);
        curr = curr->right;
    }
    free_stack(s);
}

void post_order_iter(Node *root, visit_func visitor)
{
    if (!root)
        return;
    Stack *s1 = create_stack();
    Stack *s2 = create_stack();
    if (!s1 || !s2)
    {
        free_stack(s1);
        free_stack(s2);
        return;
    }
    push(s1, (Item)root);
    while (!is_empty(s1))
    {
        Node *curr = (Node *)pop(s1);
        push(s2, (Item)curr);
        if (curr->left)
            push(s1, (Item)curr->left);
        if (curr->right)
            push(s1, (Item)curr->right);
    }
    while (!is_empty(s2))
    {
        Node *curr = (Node *)pop(s2);
        visitor(curr->data);
    }
    free_stack(s1);
    free_stack(s2);
}

void free_tree(Node *root, free_func free_item)
{
    if (!root)
        return;
    free_tree(root->left, free_item);
    free_tree(root->right, free_item);
    if (free_item)
        free_item(root->data);
    free(root);
}
