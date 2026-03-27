#ifndef WINMUSIC_H
#define WINMUSIC_H

#define _USE_MATH_DEFINES
#include <windows.h>

typedef enum
{
    NOTE_C = 0,
    NOTE_CS = 1, // C# / Db
    NOTE_D = 2,
    NOTE_DS = 3, // D# / Eb
    NOTE_E = 4,
    NOTE_F = 5,
    NOTE_FS = 6, // F# / Gb
    NOTE_G = 7,
    NOTE_GS = 8, // G# / Ab
    NOTE_A = 9,
    NOTE_AS = 10, // A# / Bb
    NOTE_B = 11,
} note_t;

typedef struct wm_context
{
    HWAVEOUT hWave;
    WAVEFORMATEX fmt;
} wm_context;

typedef struct wm_params wm_params;

wm_context *wm_init(void);
wm_context *wm_init_with_params(wm_params params);
void wm_play(wm_context *ctx, note_t note, int octave, int duration_ms);
void wm_shutdown(wm_context *ctx);

double wm_note_freq(int note, int octave);

#endif