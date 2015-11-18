#include <stdlib.h>
#include "soundpipe.h"
#include "sporth.h"
#include "base.h"

int auria_init(auria_data *gd, char *filename)
{
    gd->sr = MY_SRATE;
    gd->state = 1;
    gd->posY = 0.5;
    gd->posX = 1;
    gd->level = 0;
    gd->offset = 0;
    gd->nbars = 500;
    gd->mode = AURIA_SCROLL;
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

    /* init crossfade TODO: refactor */

    gd->cf.pos = 1;
    gd->cf.time =  0.05 * gd->sp->sr;
#ifdef USE_F310    
    f310_start(&gd->fd, auria_kontrol, gd);
#endif

    gd->accX = 0;
    gd->accY = 0;
    return 0;
}
