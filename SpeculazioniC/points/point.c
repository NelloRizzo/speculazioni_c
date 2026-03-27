#include "point.h"
#include <stddef.h>
#include <math.h>

struct punto
{
    float x;
    float y;
};

struct elemento
{
    struct punto *punto;
    struct list_item *successivo;
};

struct lista_punti
{
    struct elemento *testa;
    int totale_punti;
};

Punto crea_punto(float x, float y)
{
    Punto punto = malloc(sizeof(punto));
    if (!punto)
        return NULL;
    punto->x = x;
    punto->y = y;
    return punto;
}

float ascissa(Punto p)
{
    if (!p)
        return 0;
    return p->x;
}

float ordinata(Punto p)
{
    if (!p)
        return 0;
    return p->y;
}

float distanza(Punto p1, Punto p2)
{
    float dx = p1->x - p2->x, dy = p1->y - p2->y;
    return sqrt(dx * dx + dy * dy);
}

void sposta_punto(Punto p, float dx, float dy)
{
    if (!p)
        return;
    p->x += dx;
    p->y += dy;
}

Punti crea_lista()
{
    Punti lista = malloc(sizeof(Punti));
    if (!lista)
        return NULL;
    lista->testa = NULL;
    lista->totale_punti = 0;
    return lista;
}

void aggiungi_punto(Punti lista, Punto punto)
{
    if (!lista || !punto)
        return;
    Punto p = crea_punto(punto->x, punto->y);
    if (!p)
        return;
    struct elemento *e = malloc(sizeof(struct elemento *));
    if (!e)
    {
        free(p);
        return NULL;
    }
    e->punto = p;
    e->successivo = lista->testa;
    lista->testa = e;
    lista->totale_punti++;
}

void aggiungi_punto_in_coda(Punti lista, Punto punto)
{
    if (!lista || !punto)
        return;
    Punto p = crea_punto(punto->x, punto->y);
    if (!p)
        return;
    struct elemento *e = malloc(sizeof(struct elemento *));
    if (!e)
    {
        free(p);
        return NULL;
    }
    e->punto = p;
    struct elemento *cursore = lista->testa;
    while (cursore->successivo)
    {
        cursore = cursore->successivo;
    }
    cursore->successivo = e;
    e->successivo = NULL;
    lista->totale_punti++;
}

void libera_lista(Punti lista)
{
    if (!lista)
        return;

    struct elemento *cursore = lista->testa;
    while (cursore->successivo)
    {
        struct elemento *corrente = cursore;
        cursore = cursore->successivo;
        free(corrente->punto);
        free(corrente);
    }
    free(lista);
}

Punto centroide(Punti lista)
{
    if (!lista)
        return NULL;
    float tot_x = 0, tot_y = 0;
    struct elemento *cursore = lista->testa;
    while (cursore)
    {
        tot_x += cursore->punto->x;
        tot_y += cursore->punto->y;
    }
    return crea_punto(tot_x / lista->totale_punti, tot_y / lista->totale_punti);
}
