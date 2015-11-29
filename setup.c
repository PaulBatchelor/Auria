#include <stdlib.h>
#include "soundpipe.h"
#include "sporth.h"
#include "base.h"

int auria_init(auria_data *gd, char *filename)
{
    gd->sr = MY_SRATE;
    gd->state = 1;
    gd->state_Y = 1;
    gd->posY = 0.5;
    gd->posX = 1;
    gd->level = 0;
    gd->offset = 0;
    gd->total_bars = 500;
    gd->mode = AURIA_FREEZE;
    auria_init_audio(gd, filename);
    gd->counter = 0;
    gd->counter_speed = (unsigned int) gd->wav->size / gd->total_bars;
    gd->mincer_offset = 0;
    gd->wtpos = 0;

    unsigned int n;
    unsigned int skip = gd->wav->size / gd->total_bars;

    /* init crossfade. TODO: refactor */
    gd->cf.pos = 1;
    gd->cf.time =  0.005 * gd->sp->sr;

#ifdef USE_F310    
    f310_start(&gd->fd, auria_kontrol, gd);
#endif

    gd->accX = 0;
    gd->accY = 0;

    gd->run = 1;

    /* init linebuf */
    gd->line = (auria_cor *)malloc(sizeof(auria_cor) * gd->total_bars);
    gd->tmp_line = (auria_cor *)malloc(sizeof(auria_cor) * gd->total_bars);

    /* for testing purposes, assign random XY values */ 

    for(n = 0; n < gd->total_bars; n++) {
        gd->line[n].x = (float)n/gd->total_bars;
        gd->line[n].y = (float)n/gd->total_bars;
        gd->line[n].amp = 1;
        gd->line[n].draw_circ = 0;
    }
    gd->nbars = 0;
    gd->size_s = 0;
    gd->line_offset = 0;
    gd->tbl_pos = 0;

    gd->drawline = 0;

    gd->please_draw_circ = 0;

    gd->bar_dur = 0;

    auria_stack_create(&gd->circle_stack, AURIA_STACK_SIZE);

    auria_stack_create(&gd->line_fifo, gd->sp->sr * AURIA_BUFSIZE);
    gd->wrap = 0;
    return 0;
}
