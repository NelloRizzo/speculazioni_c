/**
 * @file vector.c
 * @brief Implementazione del vettore generico polimorfico.
 *
 * Strategia di crescita: quando la capacità è esaurita, viene raddoppiata
 * (o portata a STD_CAPACITY = 16 se era zero), garantendo complessità
 * ammortizzata O(1) per le operazioni di push.
 */

#include "vector.h"

/** Capacità iniziale predefinita per vector_create_empty e primo push */
#define STD_CAPACITY 16

/**
 * @struct item
 * @brief Wrapper attorno al puntatore generico.
 *
 * Mantenere un wrapper separato (invece di usare void** direttamente)
 * permette di aggiungere metadati per elemento in futuro senza modificare
 * l'interfaccia pubblica.
 */
struct item
{
    void *data;
};

/**
 * @struct vector
 * @brief Struttura principale del vettore.
 * @var base      Array dinamico di item
 * @var capacity  Capacità allocata (numero di item contenibili senza riallocazione)
 * @var size      Numero effettivo di elementi presenti
 * @var free_func Callback per la deallocazione degli elementi (può essere NULL)
 */
struct vector
{
    struct item *base;
    size_t capacity;
    size_t size;
    free_fn free_func;
};

/**
 * @brief Alloca un vettore con capacità iniziale e imposta la free_func.
 * @param initial_capacity Slot da preallocare
 * @param free_func        Callback di deallocazione (può essere NULL)
 * @return Vettore allocato, o NULL
 */
Vector *vector_create(size_t initial_capacity, free_fn free_func)
{
    struct item *base_item = calloc(sizeof(struct item), initial_capacity);
    if (!base_item)
        return NULL;

    struct vector *v = malloc(sizeof(struct vector));
    if (!v)
    {
        free(base_item);
        return NULL;
    }
    v->base = base_item;
    v->capacity = initial_capacity;
    v->size = 0;
    v->free_func = free_func;
    return v;
}

/**
 * @brief Alloca un vettore vuoto (capacity = 0, base = NULL).
 * @param free_func Callback di deallocazione (può essere NULL)
 * @return Vettore allocato, o NULL
 */
Vector *vector_create_empty(free_fn free_func)
{
    struct vector *v = malloc(sizeof(struct vector));
    if (!v)
        return NULL;
    v->base = NULL;
    v->capacity = 0;
    v->size = 0;
    v->free_func = free_func;
    return v;
}

/**
 * @brief Crea un vettore inizializzato copiando i puntatori da un array C.
 * @param c_vector Array sorgente
 * @param size     Numero di elementi
 * @param free_func Callback di deallocazione (può essere NULL)
 * @return Vettore allocato e popolato, o NULL
 */
Vector *vector_init(void **c_vector, size_t size, free_fn free_func)
{
    struct vector *v = vector_create(size, free_func);
    if (!v)
        return NULL;

    for (int i = 0; i < (int)size; i++)
        (v->base + i)->data = *(c_vector + i);
    v->size = size;
    return v;
}

/**
 * @brief Dealloca il vettore: libera ogni elemento con free_func,
 *        poi l'array interno e la struttura.
 * @param v Vettore da deallocare (NULL ignorato)
 */
void vector_free(Vector *v)
{
    if (!v)
        return;
    for (int i = 0; i < v->size; i++)
    {
        if (v->free_func)
            v->free_func((v->base + i)->data);
    }
    free(v->base);
    free(v);
}

/**
 * @brief Restituisce l'elemento all'indice dato.
 * @param v     Vettore (const)
 * @param index Indice (0-based)
 * @return Puntatore all'elemento, o NULL se fuori range o v è NULL
 */
void *vector_get(const Vector *v, int index)
{
    if (!v)
        return NULL;
    if (index < 0 || index >= v->size)
        return NULL;
    return (v->base + index)->data;
}

/**
 * @brief Imposta il valore all'indice dato.
 * @param v     Vettore
 * @param index Indice (0-based)
 * @param value Nuovo valore
 */
void vector_set(Vector *v, int index, void *value)
{
    if (!v || index < 0 || index >= v->size)
        return;
    (v->base + index)->data = value;
}

/**
 * @brief Applica una callback a ogni elemento del vettore.
 * @param v     Vettore (const)
 * @param apply Callback da invocare su ogni elemento
 */
void vector_for_each(const Vector *v, apply_fn apply)
{
    if (!v)
        return;
    for (int i = 0; i < v->size; i++)
        apply((v->base + i)->data);
}

/**
 * @brief Ridimensiona la capacità del vettore.
 * @param v        Vettore
 * @param new_size Nuova capacità (se minore di size, size viene troncato)
 */
void vector_resize(Vector *v, size_t new_size)
{
    if (!v)
        return;

    struct item *tmp = realloc(v->base, sizeof(struct item) * new_size);
    if (!tmp)
        return;
    v->base = tmp;
    v->capacity = new_size;
    if (v->size > new_size)
        v->size = new_size;
}

/**
 * @brief Aggiunge un elemento in coda, riallocando se necessario.
 * @param v     Vettore
 * @param value Elemento da aggiungere
 * @return 1 se successo, 0 se riallocazione fallita
 */
int vector_push(Vector *v, void *value)
{
    if (!v)
        return 0;

    if (v->size >= v->capacity)
    {
        size_t new_capacity = v->capacity ? v->capacity * 2 : STD_CAPACITY;
        struct item *tmp = realloc(v->base, sizeof(struct item) * new_capacity);
        if (!tmp)
            return 0;
        v->base = tmp;
        v->capacity = new_capacity;
    }

    (v->base + v->size)->data = value;
    v->size++;
    return 1;
}

/**
 * @brief Rimuove e restituisce l'ultimo elemento.
 * @param v Vettore
 * @return Elemento rimosso, o NULL se il vettore è vuoto
 */
void *vector_pop(Vector *v)
{
    if (!v || v->size == 0)
        return NULL;

    v->size--;
    return (v->base + v->size)->data;
}

/**
 * @brief Restituisce la capacità allocata.
 * @param v Vettore (const)
 * @return Capacità, o 0 se v è NULL
 */
int vector_capacity(const Vector *v)
{
    return v ? v->capacity : 0;
}

/**
 * @brief Restituisce il numero di elementi presenti.
 * @param v Vettore (const)
 * @return Numero di elementi, o 0 se v è NULL
 */
int vector_size(const Vector *v)
{
    return v ? v->size : 0;
}

/**
 * @brief Riduce il vettore a un singolo valore (fold sinistro).
 * @param v           Vettore (const)
 * @param reduce      Funzione di accumulazione
 * @param accumulator Valore iniziale
 * @return Accumulatore finale
 */
void *vector_reduce(const Vector *v, reduce_fn reduce, void *accumulator)
{
    if (!v)
        return accumulator;

    for (int i = 0; i < v->size; i++)
        accumulator = reduce(accumulator, (v->base + i)->data);
    return accumulator;
}

/**
 * @brief Trasforma ogni elemento con map e restituisce un nuovo vettore.
 *
 * In caso di fallimento di push, l'elemento non salvato viene deallocato
 * con free_func e il vettore parziale viene liberato, evitando memory leak.
 * @param v         Vettore sorgente (const)
 * @param map       Funzione di trasformazione
 * @param free_func Callback per deallocare gli elementi del nuovo vettore
 * @return Nuovo vettore, o NULL se v è NULL o l'allocazione fallisce
 */
Vector *vector_map(const Vector *v, map_fn map, free_fn free_func)
{
    if (!v)
        return NULL;

    Vector *result = vector_create_empty(free_func);
    if (!result)
        return NULL;

    for (int i = 0; i < v->size; i++)
    {
        void *mapped = map((v->base + i)->data);
        if (!vector_push(result, mapped))
        {
            if (free_func)
                free_func(mapped);
            vector_free(result);
            return NULL;
        }
    }
    return result;
}
