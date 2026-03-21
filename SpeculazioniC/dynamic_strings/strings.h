#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

/* ─── Strutture ──────────────────────────────────────────────────────────────
 *
 * La libreria gestisce una lista doppiamente concatenata di stringhe.
 * Ogni nodo contiene una stringa C (terminata da '\0') e i puntatori
 * al nodo precedente e successivo.
 *
 * Tutte le funzioni che restituiscono 'string *' restituiscono la TESTA
 * della lista. Il chiamante è responsabile di aggiornare il proprio
 * puntatore con il valore di ritorno:
 *
 *   string *s = string_create("Ciao");
 *   s = string_append(" mondo", s);   // aggiorna sempre s
 *
 * La memoria va liberata con string_free() sulla testa della lista.
 */

/* Una lista di stringhe */
typedef struct string
{
    char *content;       /* stringa contenuta nel nodo (terminata da '\0') */
    size_t len;          /* lunghezza della stringa (senza '\0') */
    struct string *prev; /* nodo precedente, NULL se è la testa */
    struct string *next; /* nodo successivo, NULL se è la coda */
} string;

/*
 * Risultato di una ricerca con string_find().
 * Va liberato con free_find_result() dopo l'uso.
 * ATTENZIONE: 'found' punta a un nodo della lista originale,
 * non va liberato separatamente.
 */
typedef struct find_result
{
    char *target;
    string *found;
    int position;
} find_result;

/* Libera la struttura find_result. Non libera il nodo 'found' interno alla struttura. */
void free_find_result(find_result *p);

/* ─── Tipi di funzione ───────────────────────────────────────────────────────
 *
 * Usati come parametri da visit(), string_all(), string_any(), string_fold().
 */

/* Funzione applicata a ogni nodo da visit() */
typedef void (*string_func)(const string *);
/* Predicato: restituisce 1 (vero) o 0 (falso) su un nodo */
typedef int (*string_predicate)(const string *);
/*
 * Funzione di riduzione per string_fold().
 * 'acc' è un puntatore all'accumulatore definito dal chiamante.
 * ATTENZIONE: 'acc' è void* — nessun controllo di tipo a runtime.
 * La filosofia del C è "trust the programmer".
 */
typedef void (*string_reducer)(const string *, void *acc);

/* ─── Utilità su char* ───────────────────────────────────────────────────────*/

/* Restituisce la lunghezza di una stringa C. Restituisce 0 se s è NULL. */
size_t char_array_len(const char *s);

/*
 * Cerca 'target' in 'text'.
 * Restituisce la posizione (0-based) del primo carattere del match,
 * oppure -1 se non trovato o se un argomento è NULL.
 */
int find_in_char_array(const char *text, const char *target);

/* ─── Navigazione ────────────────────────────────────────────────────────────*/
/* Ottiene il primo nodo della lista */
string *string_head(string *node);
/* Ottiene l'ultimo nodo della lista */
string *string_tail(string *node);

/* ─── Costruzione e modifica ─────────────────────────────────────────────────*/

/* Lunghezza della stringa contenuta nella lista */
size_t string_len(const string *s);

/*
 * Crea una lista con un singolo nodo contenente 's'.
 * Equivale a string_append(s, NULL).
 * Restituisce NULL se s è NULL o se l'allocazione fallisce.
 */
string *string_create(const char *s);

/*
 * Aggiunge un nuovo nodo in coda alla lista 'head'.
 * Se head è NULL, crea una nuova lista.
 * Restituisce la testa della lista, o head invariato se l'allocazione fallisce.
 *
 *   s = string_append("testo", s);
 */
string *string_append(const char *s, string *head);
/*
 * Inserisce un nuovo nodo con 's' prima del nodo 'parent'.
 * Restituisce la testa della lista.
 * Restituisce parent invariato se s è NULL o se l'allocazione fallisce.
 */
string *string_insert(const char *s, string *parent);
/*
 * Aggiunge il nodo già allocato 'node' in coda alla lista 'head'.
 * Il nodo non deve già appartenere a un'altra lista.
 * Restituisce la testa della lista.
 */
string *string_append_node(string *n, string *head);
/*
 * Clona il nodo 'node' restituendo un nuovo nodo scollegato dalla lista.
 * Il chiamante è responsabile di liberarlo (con string_free() o
 * inserendolo in una lista che verrà liberata con string_free()).
 * Restituisce NULL se node è NULL o se l'allocazione fallisce.
 */
string *string_clone_node(string *node);
/*
 * Rimuove il nodo 'node' dalla lista, liberandone la memoria.
 * Restituisce la testa della lista aggiornata (NULL se era l'unico nodo).
 * ATTENZIONE: dopo la chiamata, il puntatore 'node' non è più valido.
 */
string *string_remove_node(string *node);

/*
 * Inserisce un nuovo nodo con 's' prima del nodo 'child'.
 * Restituisce la testa della lista.
 * Restituisce NULL se s è NULL o se l'allocazione fallisce.
 */
string *string_prepend(const char *s, string *child);
/* Libera tutti i nodi della lista a partire da 's'. */
void string_free(string *s);

/* Applica 'apply' a ogni nodo della lista. Non fa nulla se apply è NULL. */
void visit(string *s, string_func apply);
/*
 * Restituisce 1 se il predicato è vero su tutti i nodi,
 * 0 se è falso su almeno uno, -1 se predicate è NULL.
 */
int string_all(const string *, string_predicate predicate);
/*
 * Restituisce 1 se il predicato è vero su almeno un nodo,
 * 0 se è falso su tutti, -1 se predicate è NULL.
 */
int string_any(const string *, string_predicate predicate);
/*
 * Applica 'reducer' su ogni nodo accumulando il risultato in 'accumulator'.
 * Non fa nulla se reducer è NULL.
 */
void string_fold(const string *s, string_reducer reducer, void *accumulator);
/*
 * Divide il contenuto del nodo 's' in posizione 'pos', creando due nodi
 * collegati. Il nodo 's' conterrà i caratteri [0, pos), il nuovo nodo
 * i caratteri [pos, len).
 * Restituisce 's', oppure NULL se pos < 0 o se l'allocazione fallisce.
 *
 * Esempio: "StateBene" con pos=5 → "State" → "Bene"
 */
string *string_split(string *s, int pos);
/* Produce una stringa in stile C, in cui vengono concatenate tutte le stringhe della lista. */
char *string_to_char_array(const string *s);
/*
 * Cerca 'target' nella lista nodo per nodo.
 * Restituisce un find_result allocato con malloc, da liberare con
 * free_find_result(). Se target non è trovato, result->found è NULL.
 * Restituisce NULL se s o target sono NULL o se l'allocazione fallisce.
 */
find_result *string_find(string *s, const char *target);

#endif