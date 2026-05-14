#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

static void print_int(Item data)
{
    printf("%d ", *(int *)data);
}

static void free_int(Item data)
{
    free(data);
}

static int *new_int(int v)
{
    int *p = malloc(sizeof(int));
    if (p)
        *p = v;
    return p;
}

static Node *build_test_tree(void)
{
    Node *n1 = create_node(new_int(1));
    Node *n2 = create_node(new_int(2));
    Node *n3 = create_node(new_int(3));
    Node *n4 = create_node(new_int(4));
    Node *n5 = create_node(new_int(5));
    Node *n6 = create_node(new_int(6));

    node_set_left(n1, n2);
    node_set_right(n1, n3);
    node_set_left(n2, n4);
    node_set_right(n2, n5);
    node_set_right(n3, n6);

    return n1;
}

int main(void)
{
    Node *root = build_test_tree();

    printf("Altezza: %d\n", tree_height(root));
    printf("Nodi: %d\n", count_nodes(root));

    printf("Livelli (BFS): ");
    level_order(root, print_int);
    printf("\n");

    printf("Pre-order iter: ");
    pre_order_iter(root, print_int);
    printf("\n");

    printf("In-order iter: ");
    in_order_iter(root, print_int);
    printf("\n");

    printf("Post-order iter: ");
    post_order_iter(root, print_int);
    printf("\n");

    free_tree(root, free_int);
    return 0;
}
