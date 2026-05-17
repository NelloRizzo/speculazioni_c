/**
 * @file vector.h
 * @brief Vettore generico polimorfico a crescita dinamica.
 *
 * Ogni elemento è un puntatore void*, permettendo di contenere qualsiasi tipo
 * di dato. La struttura interna separa la capacità allocata (capacity) dal
 * numero effettivo di elementi (size), consentendo operazioni di push/pop
 * senza riallocazioni a ogni chiamata.
 *
 * Le funzioni che accettano un puntatore Vector* gestiscono correttamente
 * il caso NULL, comportandosi come no-op o restituendo un valore sentinella.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

/** Tipo opaco del vettore. I dettagli implementativi sono nascosti in vector.c */
typedef struct vector Vector;

/** @brief Callback per visitare un elemento in sola lettura. */
typedef void (*apply_fn)(const void *);
/** @brief Callback per deallocare un elemento. */
typedef void (*free_fn)(void *);
/** @brief Callback per la riduzione: riceve accumulatore ed elemento, restituisce il nuovo accumulatore. */
typedef void *(*reduce_fn)(void *accumulator, const void *element);
/** @brief Callback per la trasformazione: riceve un elemento, restituisce un nuovo elemento allocato. */
typedef void *(*map_fn)(const void *element);

/**
 * @brief Crea un vettore con capacità iniziale data.
 * @param initial_capacity Numero di slot da preallocare
 * @param free_func        Funzione per deallocare gli elementi (NULL se non serve)
 * @return Puntatore al vettore, o NULL se l'allocazione fallisce
 */
Vector *vector_create(size_t initial_capacity, free_fn free_func);

/**
 * @brief Crea un vettore vuoto (capacity = 0, nessuna allocazione iniziale).
 * @param free_func Funzione per deallocare gli elementi (NULL se non serve)
 * @return Puntatore al vettore, o NULL se l'allocazione fallisce
 */
Vector *vector_create_empty(free_fn free_func);

/**
 * @brief Crea un vettore inizializzato copiando i puntatori da un array C.
 * @param c_vector Array di puntatori da copiare
 * @param size     Numero di elementi nell'array
 * @param free_func Funzione per deallocare gli elementi (NULL se non serve)
 * @return Puntatore al vettore, o NULL se l'allocazione fallisce
 */
Vector *vector_init(void **c_vector, size_t size, free_fn free_func);

/**
 * @brief Dealloca il vettore e tutti i suoi elementi.
 *
 * Per ogni elemento chiama free_func (se fornita), poi libera l'array
 * interno e la struttura stessa.
 * @param v Vettore da deallocare (NULL viene ignorato)
 */
void vector_free(Vector *v);

/**
 * @brief Restituisce l'elemento all'indice specificato.
 * @param v     Vettore (const)
 * @param index Indice dell'elemento (0-based)
 * @return Puntatore all'elemento, o NULL se fuori range o v è NULL
 */
void *vector_get(const Vector *v, int index);

/**
 * @brief Imposta il valore all'indice specificato.
 * @param v     Vettore
 * @param index Indice dell'elemento (0-based)
 * @param value Nuovo valore
 */
void vector_set(Vector *v, int index, void *value);

/**
 * @brief Applica una funzione a ogni elemento del vettore, in ordine.
 * @param v     Vettore (const)
 * @param apply Funzione da chiamare su ogni elemento
 */
void vector_for_each(const Vector *v, apply_fn apply);

/**
 * @brief Ridimensiona la capacità del vettore.
 *
 * Se la nuova capacità è minore del numero di elementi correnti,
 * gli elementi in eccesso vengono scartati (size viene troncato).
 * @param v        Vettore
 * @param new_size Nuova capacità
 */
void vector_resize(Vector *v, size_t new_size);

/**
 * @brief Aggiunge un elemento in coda al vettore.
 *
 * Se la capacità è esaurita, il vettore viene riallocato con capacità
 * raddoppiata (o STD_CAPACITY = 16 se era zero).
 * @param v     Vettore
 * @param value Elemento da aggiungere
 * @return 1 se successo, 0 se la riallocazione fallisce
 */
int vector_push(Vector *v, void *value);

/**
 * @brief Rimuove e restituisce l'ultimo elemento del vettore.
 * @param v Vettore
 * @return L'elemento rimosso, o NULL se il vettore è vuoto
 */
void *vector_pop(Vector *v);

/**
 * @brief Restituisce la capacità allocata del vettore.
 * @param v Vettore (const)
 * @return Capacità, o 0 se v è NULL
 */
int vector_capacity(const Vector *v);

/**
 * @brief Restituisce il numero di elementi presenti nel vettore.
 * @param v Vettore (const)
 * @return Numero di elementi, o 0 se v è NULL
 */
int vector_size(const Vector *v);

/**
 * @brief Riduce il vettore a un singolo valore applicando iterativamente
 *        una funzione di accumulazione (come Array.prototype.reduce in JS).
 * @param v           Vettore (const)
 * @param reduce      Funzione di accumulazione
 * @param accumulator Valore iniziale dell'accumulatore
 * @return Valore finale dell'accumulatore
 */
void *vector_reduce(const Vector *v, reduce_fn reduce, void *accumulator);

/**
 * @brief Applica una funzione di trasformazione a ogni elemento,
 *        restituendo un nuovo vettore con i risultati.
 *
 * In caso di fallimento a metà, gli elementi già trasformati vengono
 * deallocati con free_func e il vettore parziale viene liberato.
 * @param v         Vettore sorgente (const)
 * @param map       Funzione di trasformazione
 * @param free_func Funzione per deallocare i nuovi elementi (può essere NULL)
 * @return Nuovo vettore, o NULL se l'allocazione fallisce o v è NULL
 */
Vector *vector_map(const Vector *v, map_fn map, free_fn free_func);

#endif
