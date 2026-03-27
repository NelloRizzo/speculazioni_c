#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>

#include "winmusic.h"

#define SAMPLE_RATE 44100
#define CHANNELS 1
#define FREQ 440
#define DURATION_MS 1000
#define BIT_PER_SAMPLE 16

struct wm_params
{
    int sample_rate;
    int channels;
    int freq;
    int duration_ms;
    int bit_per_sample;
};

static wm_context *openWave(wm_context *ctx)
{
    MMRESULT res = waveOutOpen(&ctx->hWave, WAVE_MAPPER, &ctx->fmt, 0, 0, 0);
    if (res != MMSYSERR_NOERROR)
    {
        free(ctx);
        return NULL;
    }
    return ctx;
}

double wm_note_freq(int note, int octave)
{
    double semitones = (note - NOTE_A) + (octave - 4) * 12;
    return 440.0 * pow(2.0, semitones / 12.0);
}

wm_context *wm_init(void)
{
    wm_context *ctx = malloc(sizeof(wm_context));
    if (!ctx)
        return NULL;

    ctx->fmt.wFormatTag = WAVE_FORMAT_PCM;
    ctx->fmt.nChannels = CHANNELS;
    ctx->fmt.nSamplesPerSec = SAMPLE_RATE;
    ctx->fmt.wBitsPerSample = BIT_PER_SAMPLE;
    ctx->fmt.nBlockAlign = CHANNELS * (BIT_PER_SAMPLE / 8);
    ctx->fmt.nAvgBytesPerSec = ctx->fmt.nBlockAlign * SAMPLE_RATE;
    ctx->fmt.cbSize = 0;
    return openWave(ctx);
}

wm_context *wm_init_with_params(wm_params params)
{
    wm_context *ctx = malloc(sizeof(wm_context));
    if (!ctx)
        return NULL;

    ctx->fmt.wFormatTag = WAVE_FORMAT_PCM;
    ctx->fmt.nChannels = params.channels;
    ctx->fmt.nSamplesPerSec = params.sample_rate;
    ctx->fmt.wBitsPerSample = params.bit_per_sample;
    ctx->fmt.nBlockAlign = params.channels * (params.bit_per_sample / 8);
    ctx->fmt.nAvgBytesPerSec = ctx->fmt.nBlockAlign * params.sample_rate;
    ctx->fmt.cbSize = 0;
    return openWave(ctx);
}
void wm_play(wm_context *ctx, note_t note, int octave, int duration_ms)
{
    if (!ctx)
        return;

    int n_samples = ctx->fmt.nSamplesPerSec * duration_ms / 1000;
    double freq = wm_note_freq(note, octave);
    int16_t samples[n_samples];
    for (int i = 0; i < n_samples; i++)
    {
        double t = (double)i / ctx->fmt.nSamplesPerSec; // tempo in secondi
        samples[i] = (int16_t)(sin(2.0 * M_PI * freq * t) * 32767.0);
    }
    int fade_samples = ctx->fmt.nSamplesPerSec * 10 / 1000; // 10ms di fade
    for (int i = 0; i < fade_samples; i++)
    {
        float factor = 1.0f - (float)i / fade_samples;
        samples[n_samples - fade_samples + i] *= factor;
    }
    WAVEHDR hdr;
    hdr.lpData = (LPSTR)samples;
    hdr.dwBufferLength = n_samples * (ctx->fmt.wBitsPerSample / 8);
    hdr.dwFlags = 0;
    waveOutPrepareHeader(ctx->hWave, &hdr, sizeof(WAVEHDR));
    waveOutWrite(ctx->hWave, &hdr, sizeof(WAVEHDR));
    while (!(hdr.dwFlags & WHDR_DONE))
        Sleep(10);

    waveOutUnprepareHeader(ctx->hWave, &hdr, sizeof(WAVEHDR));
}

void wm_shutdown(wm_context *ctx)
{
    if (!ctx)
        return;
    waveOutClose(ctx->hWave);
    free(ctx);
}
