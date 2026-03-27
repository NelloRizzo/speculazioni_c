#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include <stdlib.h>

#include "tracks.h"

#define WM_TRACK_INITIAL_CAPACITY 44100 // 1 secondo iniziale

wm_track *wm_track_create(void)
{
    wm_track *track = malloc(sizeof(wm_track));
    if (!track)
        return NULL;
    track->capacity = WM_TRACK_INITIAL_CAPACITY;
    track->n_samples = 0;
    track->buffer = malloc(track->capacity * sizeof(int16_t));
    if (!track->buffer)
    {
        free(track);
        return NULL;
    }
    return track;
}

void wm_track_add_note(wm_context *ctx, wm_track *track, note_t note, int octave, int duration_ms)
{
    if (!ctx || !track)
        return;

    int n_samples = ctx->fmt.nSamplesPerSec * duration_ms / 1000;

    while (track->n_samples + n_samples > track->capacity)
        track->capacity *= 2;
    int16_t *tmp_buffer = realloc(track->buffer, track->capacity * sizeof(int16_t));
    if (!tmp_buffer)
    {
        return;
    }
    track->buffer = tmp_buffer;

    double freq = wm_note_freq(note, octave);
    for (int i = 0; i < n_samples; i++)
    {
        double t = (double)i / ctx->fmt.nSamplesPerSec;
        *(track->buffer + track->n_samples + i) = (int16_t)(sin(2.0 * M_PI * freq * t) * 32767.0);
    }
    // fade in — usa track->n_samples come base, non ancora aggiornato
    int fade_samples = ctx->fmt.nSamplesPerSec * 10 / 1000;
    for (int i = 0; i < fade_samples; i++)
    {
        float factor = (float)i / fade_samples;
        *(track->buffer + track->n_samples + i) *= factor;
    }

    // fade out
    for (int i = 0; i < fade_samples; i++)
    {
        float factor = 1.0f - (float)i / fade_samples;
        *(track->buffer + track->n_samples + n_samples - fade_samples + i) *= factor;
    }

    // aggiorna DOPO i fade
    track->n_samples += n_samples;
}

void wm_track_add_silence(wm_context *ctx, wm_track *track, int duration_ms)
{
}

void wm_track_play(wm_context *ctx, wm_track *track)
{
    if (!ctx || !track || !track->buffer || track->n_samples == 0)
        return;

    WAVEHDR hdr;
    hdr.lpData = (LPSTR)track->buffer;
    hdr.dwBufferLength = track->n_samples * (ctx->fmt.wBitsPerSample / 8);
    hdr.dwFlags = 0;
    waveOutPrepareHeader(ctx->hWave, &hdr, sizeof(WAVEHDR));
    waveOutWrite(ctx->hWave, &hdr, sizeof(WAVEHDR));
    while (!(hdr.dwFlags & WHDR_DONE))
        Sleep(10);

    waveOutUnprepareHeader(ctx->hWave, &hdr, sizeof(WAVEHDR));
}

void wm_track_free(wm_track *track)
{
    if (!track)
        return;
    free(track->buffer);
    free(track);
}
