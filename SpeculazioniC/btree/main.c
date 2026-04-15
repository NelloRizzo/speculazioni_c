#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

// Funzione di confronto per interi
int compare_ints(void *a, void *b)
{
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return val_a - val_b;
}

// Funzione per stampare i dati durante la visita
void print_int(void *data)
{
    if (data)
    {
        printf("%d ", *(int *)data);
    }
}

// Funzione helper per creare un intero allocato dinamicamente
int *new_int(int value)
{
    int *p = malloc(sizeof(int));
    if (p)
        *p = value;
    return p;
}

int main(void)
{
    // 1. Creazione dell'albero (senza duplicati)
    // Usiamo 'free' standard come free_func perché i dati sono semplici malloc
    BTree *tree = create_btree(compare_ints, free, 0);

    if (!tree)
    {
        fprintf(stderr, "Errore nella creazione dell'albero\n");
        return 1;
    }

    printf("--- Test Inserimento ---\n");
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++)
    {
        printf("Inserisco: %d\n", values[i]);
        btree_append(tree, new_int(values[i]));
    }

    // 2. Test duplicato (con allow_duplicates = 0 non dovrebbe aumentare la size)
    printf("\nTentativo inserimento duplicato (50)...\n");
    int *val = new_int(50);
    if (!btree_append(tree, val))
        free(val);
    printf("Dimensione albero (attesa 7): %d\n", btree_size(tree));

    // 3. Test Visite
    printf("\n--- Test Visite ---\n");

    printf("In-order (ordinata): ");
    visit_btree(tree, print_int);
    printf("\n");

    printf("Pre-order (radice per prima): ");
    pre_visit_btree(tree, print_int);
    printf("\n");

    printf("Post-order (foglie per prime): ");
    post_visit_btree(tree, print_int);
    printf("\n");

    // 4. Test con Duplicati Permessi
    printf("\n--- Test Duplicati ---\n");
    BTree *dup_tree = create_btree(compare_ints, free, 1);
    btree_append(dup_tree, new_int(10));
    btree_append(dup_tree, new_int(10));
    btree_append(dup_tree, new_int(10));
    printf("Dimensione albero con duplicati (attesa 3): %d\n", btree_size(dup_tree));
    printf("Contenuto: ");
    visit_btree(dup_tree, print_int);
    printf("\n");

    // 5. Pulizia Memoria
    printf("\n--- Pulizia Memoria ---\n");
    destroy_btree(tree);
    destroy_btree(dup_tree);
    printf("Memoria liberata con successo.\n");

    return 0;
}