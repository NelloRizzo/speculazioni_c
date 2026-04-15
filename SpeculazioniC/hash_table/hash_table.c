#define _GNU_SOURCE // per strdup

#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

typedef struct entry
{
    char *key;          // La chiave
    void *value;        // Il valore associato
    struct entry *next; // Per gestire le collisioni (chaining)
} Entry;

struct hash_table
{
    free_func free_fn; // Funzione per liberare i dati nella tabella
    hash_func hash_fn; // Funzione per il calcolo della chiave
    int size;          // Numero di bucket
    int count;         // Quanti elementi ci sono davvero
    Entry **buckets;   // Array di puntatori a Entry
};

unsigned long hash_djb2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = (hash << 5) + hash + (unsigned long)(c); // hash * 33 + c
    return hash;
}

unsigned long hash_sdbm(unsigned char *str)
{
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
    {
        // hash * 65599 + c
        hash = (unsigned long)c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

#define FNV_OFFSET_BASIS 2166136261U
#define FNV_PRIME 16777619U

unsigned long hash_fnv1a(unsigned char *str)
{
    unsigned int hash = FNV_OFFSET_BASIS;
    while (*str)
    {
        hash ^= (unsigned int)(*str++);
        hash *= FNV_PRIME;
    }
    return hash;
}

HashTable *create_hash_table_custom(int buckets, hash_func hash, free_func free_fn)
{
    HashTable *t = malloc(sizeof(HashTable));
    if (!t)
        return NULL;
    t->count = 0;
    t->size = buckets;
    t->hash_fn = hash;
    t->free_fn = free_fn;
    t->buckets = calloc((size_t)buckets, sizeof(Entry *));
    if (!t->buckets)
    {
        free(t);
        return NULL;
    }
    return t;
}

HashTable *create_hash_table(free_func free_fn)
{
    return create_hash_table_custom(16, hash_sdbm, free_fn);
}

int hash_table_size(const HashTable *table)
{
    return table ? table->size : 0;
}

int hash_table_count(const HashTable *table)
{
    return table ? table->count : 0;
}

void hash_table_insert(HashTable *table, const char *key, void *value)
{
    if (!table || !key)
        return;

    // Se il numero di elementi supera il 75% della dimensione, raddoppia
    if ((float)table->count / (float)table->size > 0.75f)
    {
        hash_table_resize(table);
    }

    unsigned long h = table->hash_fn((unsigned char *)key);
    int index = (int)(h % (unsigned long)table->size);
    Entry *curr = table->buckets[index];
    while (curr != NULL) // gestione delle collisioni
    {
        if (strcmp(curr->key, key) == 0)
        {
            // Trovata! Liberiamo il vecchio valore se necessario
            if (table->free_fn && curr->value)
            {
                table->free_fn(curr->value);
            }
            curr->value = value;
            return; // Operazione conclusa
        }
        curr = curr->next;
    }

    // 2. Se arriviamo qui, la chiave non esiste: creiamo una nuova Entry
    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry)
        return;

    new_entry->key = strdup(key); // duplica la chiave
    new_entry->value = value;

    // 3. Inserimento in TESTA al bucket: si tratta di un'operazione O(1)
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;

    table->count++;
}

void free_bucket(Entry *bucket, free_func free_fn)
{
    while (bucket)
    {
        Entry *next = bucket->next;
        if (free_fn && bucket->value)
            free_fn(bucket->value);
        free(bucket->key);
        free(bucket);
        bucket = next;
    }
}

void hash_table_free(HashTable *table)
{
    if (!table)
        return;

    // Scorriamo ogni bucket dell'array
    for (int i = 0; i < table->size; i++)
    {
        if (table->buckets[i])
        {
            free_bucket(table->buckets[i], table->free_fn);
        }
    }

    // Libera l'array di puntatori (quello creato con calloc)
    free(table->buckets);

    // Libera la struttura principale
    free(table);
}

void *hash_table_search(const HashTable *table, const char *key)
{
    if (!table || !key)
        return NULL;

    unsigned long h = table->hash_fn((unsigned char *)key);
    int index = (int)(h % (unsigned long)table->size);
    Entry *curr = table->buckets[index];
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0) // trovata!
        {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

int hash_table_remove(HashTable *table, const char *key)
{
    if (!table || !key)
        return 0;

    unsigned long h = table->hash_fn((unsigned char *)key);
    int index = (int)(h % (unsigned long)table->size);

    Entry *curr = table->buckets[index];
    Entry *prev = NULL;

    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
        {
            // 1. "Scolleghiamo" il nodo dalla lista
            if (prev == NULL)
            {
                // Il nodo da rimuovere è il primo del bucket
                table->buckets[index] = curr->next;
            }
            else
            {
                // Il nodo è in mezzo o alla fine
                prev->next = curr->next;
            }

            // 2. Liberiamo la memoria
            if (table->free_fn && curr->value)
            {
                table->free_fn(curr->value);
            }
            free(curr->key);
            free(curr);

            table->count--;
            return 1; // Rimosso con successo
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; // Chiave non trovata
}

void hash_table_resize(HashTable *table)
{
    int old_size = table->size;
    int new_size = old_size * 2;

    // 1. Alloca il nuovo array di bucket (tutti NULL grazie a calloc)
    Entry **new_buckets = calloc((size_t)new_size, sizeof(Entry *));
    if (!new_buckets)
        return; // Se fallisce, continuiamo con la vecchia (lenta)

    // 2. Ricalcola gli indici per ogni Entry esistente
    for (int i = 0; i < old_size; i++)
    {
        Entry *curr = table->buckets[i];
        while (curr)
        {
            Entry *next = curr->next; // Salviamo il prossimo prima di cambiare i puntatori

            // Calcoliamo il nuovo indice con la nuova dimensione
            unsigned long h = table->hash_fn((unsigned char *)curr->key);
            int new_index = (int)(h % (unsigned long)new_size);

            // Inserimento in testa nel nuovo bucket
            curr->next = new_buckets[new_index];
            new_buckets[new_index] = curr;

            curr = next;
        }
    }

    // 3. Sostituiamo il vecchio array con il nuovo
    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;
}