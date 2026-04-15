#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// Funzione helper per creare un intero sulla heap
int *nuovo_intero(int v)
{
    int *p = malloc(sizeof(int));
    if (p)
        *p = v;
    return p;
}

int main(void)
{
    printf("--- Test HashTable ---\n");

    // 1. Creazione della tabella
    // Usiamo una dimensione iniziale piccola (4) per forzare il resize velocemente
    // Usiamo 'free' come funzione per liberare i valori (gli int*)
    HashTable *ht = create_hash_table_custom(4, hash_fnv1a, free);

    if (!ht)
    {
        fprintf(stderr, "Errore creazione tabella\n");
        return 1;
    }

    // 2. Test Inserimento
    printf("Inserimento elementi...\n");
    char *nomi[] = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace"};
    for (int i = 0; i < 7; i++)
    {
        hash_table_insert(ht, nomi[i], nuovo_intero(i + 100));
        printf("Inserito %s (Size attuale: %d, Count: %d)\n", nomi[i], hash_table_size(ht), hash_table_count(ht));
    }

    // 3. Test Ricerca
    printf("\n--- Test Ricerca ---\n");
    char *cerca = "Charlie";
    int *valore = (int *)hash_table_search(ht, cerca);
    if (valore)
    {
        printf("Trovato %s: %d\n", cerca, *valore);
    }
    else
    {
        printf("%s non trovato!\n", cerca);
    }

    // 4. Test Aggiornamento (Sostituzione valore)
    printf("\nAggiornamento valore per 'Alice'...\n");
    hash_table_insert(ht, "Alice", nuovo_intero(999));
    valore = (int *)hash_table_search(ht, "Alice");
    printf("Nuovo valore Alice: %d (Count dovrebbe restare invariato: %d)\n", *valore, hash_table_count(ht));

    // 5. Test Rimozione
    printf("\n--- Test Rimozione ---\n");
    if (hash_table_remove(ht, "Bob"))
    {
        printf("Bob rimosso. Count: %d\n", hash_table_count(ht));
    }
    if (hash_table_search(ht, "Bob") == NULL)
    {
        printf("Conferma: Bob non esiste più.\n");
    }

    // 6. Test Ridimensionamento (Resize)
    // Aggiungiamo molti elementi per vedere se la tabella cresce bene
    printf("\nInserimento massivo per testare il Resize...\n");
    for (int i = 0; i < 20; i++)
    {
        char chiave[20];
        sprintf(chiave, "Chiave_%d", i);
        hash_table_insert(ht, chiave, nuovo_intero(i));
    }
    printf("Stato Finale: Size: %d, Count: %d\n", hash_table_size(ht), hash_table_count(ht));

    // 7. Distruzione e controllo leak
    printf("\n--- Distruzione Tabella ---\n");
    hash_table_free(ht);
    printf("Memoria liberata.\n");

    return 0;
}