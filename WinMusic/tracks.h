#ifndef TRACKS_H
#define TRACKS_H

#include <stdint.h>

#include "winmusic.h"

typedef struct wm_track {
    int16_t *buffer;      // buffer PCM crescente
    size_t   n_samples;   // campioni scritti finora
    size_t   capacity;    // campioni allocati
} wm_track;

// crea una traccia vuota
wm_track *wm_track_create(void);

// aggiunge una nota alla fine della traccia
void wm_track_add_note(wm_context *ctx, wm_track *track, note_t note, int octave, int duration_ms);

// aggiunge silenzio (pausa)
void wm_track_add_silence(wm_context *ctx, wm_track *track, int duration_ms);

// suona tutta la traccia
void wm_track_play(wm_context *ctx, wm_track *track);

// libera
void wm_track_free(wm_track *track);

#endif