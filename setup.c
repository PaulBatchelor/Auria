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
    gd->nbars = 500;
    gd->mode = AURIA_FREEZE;
    gd->soundbars = (float *)malloc(sizeof(float) * gd->nbars);
    auria_init_audio(gd, filename);
    gd->counter = 0;
    gd->counter_speed = (unsigned int) gd->wav->size / gd->nbars;
    gd->mincer_offset = 0;
    gd->wtpos = 0;
    gd->sum = 0;

    unsigned int n;
    unsigned int skip = gd->wav->size / gd->nbars;
    SPFLOAT out = 0;

    for(n = 0; n < gd->nbars; n++) {
        gd->soundbars[n] = gd->wav->tbl[n * skip];
    }

    gd->soundbars[0] = 1;

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
    gd->line = (auria_cor *)malloc(sizeof(auria_cor) * gd->nbars);
    gd->tmp_line = (auria_cor *)malloc(sizeof(auria_cor) * gd->nbars);

    /* for testing purposes, assign random XY values */ 

    for(n = 0; n < gd->nbars; n++) {
        gd->line[n].x = (float)n/gd->nbars;
        gd->line[n].y = (float)n/gd->nbars;
        gd->line[n].amp = 1;
    }
    gd->dur = 0;
    gd->size_s = 0;
    gd->line_offset = 0;
    gd->tbl_pos = 0;

    gd->drawline = 0;
    return 0;
}

