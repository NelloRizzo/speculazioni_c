#ifndef POINT_H
#define POINT_H

typedef struct punto* Punto;
typedef struct lista_punti *Punti;

Punto crea_punto(float, float);
float ascissa(Punto);
float ordinata(Punto);
float distanza(Punto, Punto);
void sposta_punto(Punto, float, float);

Punti crea_lista();
void aggiungi_punto(Punti lista, Punto punto);
void aggiungi_punto_in_coda(Punti lista, Punto punto);
void libera_lista(Punti lista);
Punti centroide(Punti lista);

#endif