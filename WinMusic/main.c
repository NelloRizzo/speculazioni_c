#include "winmusic.h"
#include "tracks.h"

int main(void)
{
    wm_context *ctx = wm_init();
    wm_track *track_a = wm_track_create();
    wm_track_add_note(ctx, track_a, NOTE_C, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_D, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_E, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_F, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_G, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_A, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_B, 4, 400);
    wm_track_add_note(ctx, track_a, NOTE_C, 5, 400);

    wm_track_play(ctx, track_a);
    wm_shutdown(ctx);
}