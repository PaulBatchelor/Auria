#include <soundpipe.h>
#include <sporth.h>
#include <math.h>
#include "base.h"

#define LENGTH(x) ((int)(sizeof(x) / sizeof *(x)))

int auria_init_audio(auria_data *gd, char *filename)
{
    sp_createn(&gd->sp, 2);
    sp_data *sp = gd->sp;
    gd->sp->sr = gd->sr;
    //printf("loading file %s\n", filename);
    //sp_ftbl_loadfile(sp, &gd->wav, filename);
    sp_ftbl_create(sp, &gd->wav, sp->sr * 10);
    auria_mincer_create(&gd->mincer);
    auria_mincer_init(sp, gd->mincer, gd->wav);
    sp_port_create(&gd->portX);
    sp_port_init(sp, gd->portX, 0.05);
    sp_port_create(&gd->portY);
    sp_port_init(sp, gd->portY, 0.01);
    gd->onedsr = 1.0 / sp->sr;

    sp_port_create(&gd->rms_smooth);
    sp_port_init(sp, gd->rms_smooth, 0.06);
    sp_rms_create(&gd->rms);
    sp_rms_init(sp, gd->rms);

    /* init plumber/sporth */
    plumber_data *pd = &gd->pd;

    plumber_init(pd);
    printf("opening file %s\n", filename);

    pd->fp = fopen(filename, "r");

    if(pd->fp == NULL) {
        fprintf(stderr, "There was a problem opening the file %s.\n", filename);
        /*TODO: add better error handling */
    }

    pd->sp = sp;
    pd->nchan = 1;

    plumber_register(pd);

    if(plumber_parse(pd) == PLUMBER_OK) {
        plumber_compute(pd, PLUMBER_INIT);
        pd->sporth.stack.pos = 0;
    }
    return 0;
}

int auria_destroy_audio(auria_data *gd)
{
    sp_data *sp = gd->sp;
    sp_ftbl_destroy(&gd->wav);
    auria_mincer_destroy(&gd->mincer);
    sp_port_destroy(&gd->portX);
    sp_port_destroy(&gd->portY);
    sp_rms_destroy(&gd->rms);
    sp_port_destroy(&gd->rms_smooth);
    sp_destroy(&sp);
    plumber_clean(&gd->pd);
    return 0;
}

static float gen_scale(float pos) {
    float base = sp_midi2cps(60);
    float out = 0;
    int scale[] = {
        -12, -10, -8, -5, -3,
        0, 2, 4, 7, 9, 12
    };

    out = sp_midi2cps(60 + scale[(int)round((LENGTH(scale) - 1) * pos)]) / base;

    return out;
}

int auria_compute_audio(auria_data *gd)
{
    sp_data *sp = gd->sp;
    SPFLOAT sporth_out = 0;
    SPFLOAT mincer_out = 0;
    SPFLOAT out = 4;
    SPFLOAT posX = 0;
    SPFLOAT posY = 0;
    SPFLOAT pitch = 1;
    SPFLOAT pitch_port = 1;
    SPFLOAT rms = 0;
    SPFLOAT rms_smooth = 0;
    SPFLOAT mix = 0;
    if(gd->mode == AURIA_PLEASE_FREEZE) gd->mode = AURIA_FREEZE;
    if(gd->mode == AURIA_PLEASE_REPLAY) {
        gd->mode = AURIA_REPLAY;
        gd->wtpos = gd->mincer->wtpos - 1;
    }
    int mode = gd->mode;
    if(gd->state_Y == 1) {
        pitch = 2 * (1 - gd->posY);
        //pitch = gen_scale(1 - gd->posY);
    } else {
        pitch = 1;
    }

    sp_port_compute(sp, gd->portX, &gd->posX, &posX);
    gd->mincer->time = posX * gd->wav->size * gd->onedsr;

    sp_port_compute(sp, gd->portY, &pitch, &pitch_port);
    gd->mincer->pitch = pitch_port;
    auria_mincer_compute(sp, gd->mincer, NULL, &mincer_out, gd->mincer_offset);
    sp_rms_compute(sp, gd->rms, &mincer_out, &rms);
    sp_port_compute(sp, gd->rms_smooth, &rms, &rms_smooth);
    gd->level = rms_smooth * 4;
        
    if(mode == AURIA_SCROLL) {
        plumber_compute(&gd->pd, PLUMBER_COMPUTE);
        sporth_out = sporth_stack_pop_float(&gd->pd.sporth.stack);
        gd->wav->tbl[(gd->mincer_offset + gd->wav->size) % gd->wav->size] = sporth_out;
        out = sporth_out;
    } else if(mode == AURIA_FREEZE) {
        out = mincer_out;
    } else if (mode == AURIA_REPLAY) {
        if( gd->wtpos + 1 > gd->wav->size) {
            gd->mode = AURIA_SCROLL;
            printf("wave position is %d wave size is %d\n", gd->wtpos, gd->wav->size);
        } 
        unsigned int t = ((gd->wtpos + gd->mincer_offset - 1) % gd->wav->size);
        //printf("%d %d\n", t, gd->last);
        gd->last = t;
        out = gd->wav->tbl[t];
        gd->posX = (float)(gd->wtpos) / gd->wav->size;
        gd->wtpos++; 

    }

    if(mode == AURIA_SCROLL) {
        if(gd->counter == 0) {
            gd->offset = (gd->offset + 1) % gd->nbars;
            gd->soundbars[(gd->offset + (gd->nbars -1)) % gd->nbars] = out;
        }
        gd->counter = (gd->counter + 1) % gd->counter_speed;
        gd->mincer_offset = (gd->mincer_offset + 1) % gd->wav->size;
    }
   
    sp->out[0] = out;
    sp->out[1] = out;
}
