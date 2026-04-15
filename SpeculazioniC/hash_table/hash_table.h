#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct hash_table HashTable;

typedef unsigned long (*hash_func)(unsigned char *);
typedef void (*free_func)(void *);

/*
    DJB2 (di Dan Bernstein)

    È la più famosa in assoluto. È incredibilmente semplice e funziona sorprendentemente bene con le stringhe ASCII,
    grazie alle proprietà del numero 33 e del numero magico 5381.
*/
unsigned long hash_djb2(unsigned char *str);

/*
    SDBM (usata nel database SDBM)

    Questa è una funzione molto "aggressiva" nel rimescolare i bit.
    È ottima per distribuire bene set di dati che hanno prefissi simili (es. indirizzi IP o percorsi di file)
*/
unsigned long hash_sdbm(unsigned char *str);
/*
    FNV-1a (Fowler-Noll-Vo)

    È la preferita di molti programmatori moderni.
    A differenza di DJB2, elabora i bit in modo che ogni byte della stringa abbia un impatto immediato e forte su tutto il risultato.
    È molto veloce e ha un tasso di collisioni bassissimo.
*/
unsigned long hash_fnv1a(unsigned char *str);

/*
    Costruttore per una HashTable.
*/
HashTable *create_hash_table_custom(int buckets, hash_func hash, free_func free_fn);
/*
    Factory per la costruzione di una HashTable con 16 buckets e funzione hash SBDM.
*/
HashTable *create_hash_table(free_func free_fn);
int hash_table_size(const HashTable *table);
int hash_table_count(const HashTable *table);
void hash_table_insert(HashTable *table, const char *key, void *value);
void hash_table_free(HashTable *table);
int hash_table_remove(HashTable *table, const char *key);
void *hash_table_search(const HashTable *table, const char *key);
void hash_table_resize(HashTable *table);
#endif