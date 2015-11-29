#include <soundpipe.h>
#include <sporth.h>
#include <math.h>
#include "base.h"

#define LENGTH(x) ((int)(sizeof(x) / sizeof *(x)))
#define min(a, b) (a < b) ? a : b


int auria_init_audio(auria_data *gd, char *filename)
{
    sp_createn(&gd->sp, 2);
    sp_data *sp = gd->sp;
    gd->sp->sr = gd->sr;
    //printf("loading file %s\n", filename);
    //sp_ftbl_loadfile(sp, &gd->wav, filename);
    sp_ftbl_create(sp, &gd->wav, sp->sr * AURIA_BUFSIZE);
    auria_mincer_create(&gd->mincer);
    auria_mincer_init(sp, gd->mincer, gd->wav);
    sp_port_create(&gd->portX);
    sp_port_init(sp, gd->portX, 0.5);
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
int auria_compute_audio(auria_data *gd)
{
    sp_data *sp = gd->sp;
    SPFLOAT sporth_out = 0;
    SPFLOAT mincer_out = 0;
    SPFLOAT wt_out = 0;
    SPFLOAT out = 4;
    SPFLOAT time_control = 0;
    SPFLOAT rms = 0;
    SPFLOAT rms_smooth = 0;
    if(gd->mode == AURIA_PLEASE_FREEZE) gd->mode = AURIA_FREEZE;
    if(gd->mode == AURIA_PLEASE_REPLAY) {
        gd->mode = AURIA_REPLAY;
        gd->wtpos = gd->mincer->wtpos - 1;
    }
    int mode = gd->mode;

    gd->posX = gd->posX + gd->accX;
    gd->posY = gd->posY + gd->accY;
    
    gd->pd.p[0] = 2 * gd->posX - 1;
    gd->pd.p[1] = 2 * gd->posY - 1;
    

    /*TODO: refactor */
    if(gd->posX > 1) gd->posX = 1;
    else if(gd->posX < 0) gd->posX = 0;

    if(gd->posY > 1) gd->posY = 1;
    else if(gd->posY < 0) gd->posY = 0;

    sp_port_compute(sp, gd->portY, &gd->posY, &time_control);

    gd->mincer->time = time_control * (gd->size_s) * gd->onedsr;
    auria_mincer_compute(sp, gd->mincer, NULL, &mincer_out, gd->mincer_offset);
    
    unsigned int t = ((gd->wtpos + gd->mincer_offset - 1) % gd->wav->size);
    wt_out = gd->wav->tbl[t];

    if(mode == AURIA_SCROLL) {
        plumber_compute(&gd->pd, PLUMBER_COMPUTE);
        sporth_out = sporth_stack_pop_float(&gd->pd.sporth.stack);
        //uint32_t pos = (gd->size_s - gd->mincer_offset) % gd->wav->size;
        gd->wav->tbl[gd->tbl_pos] 
            = sporth_out;
        out = sporth_out;
    } else if(mode == AURIA_FREEZE) {
        out = auria_cf(&gd->cf, wt_out, mincer_out);
    } else if (mode == AURIA_REPLAY) {
        if( gd->wtpos + 1 > gd->wav->size) {
            gd->mode = AURIA_SCROLL;
            printf("wave position is %d wave size is %ld\n", gd->wtpos, gd->wav->size);
        } 
        out = auria_cf(&gd->cf, mincer_out, wt_out);
        gd->posX = (float)(gd->wtpos) / gd->wav->size;
        gd->wtpos++; 

    }

    if(gd->pd.p[3] == 1) {
        gd->please_draw_circ = 1;
    }


    if(mode == AURIA_SCROLL) {
        //gd->sum += fabs(out); 
        if(gd->counter == 0) {
            //auria_cor *cor = &gd->line[gd->offset];
            //cor->x = gd->posX;
            //cor->y = gd->posY;
            //cor->amp = (float) gd->sum / gd->counter_speed;
            //if(gd->size_s >= gd->wav->size) {
            //    gd->line_offset = (gd->line_offset + 1) % gd->total_bars;
            //}
            //gd->offset = (gd->offset + 1) % gd->total_bars;
            //gd->soundbars[(gd->offset + (gd->total_bars -1)) % gd->total_bars] = (float) gd->sum / gd->counter_speed;
            //gd->sum = 0;
            //gd->dur++;
            //gd->dur = min(gd->dur, gd->total_bars - 1);
            gd->drawline = 1;
        }
        gd->counter = (gd->counter + 1) % gd->counter_speed;

        gd->size_s = min((gd->size_s + 1), gd->wav->size);
        if(gd->size_s >= gd->wav->size) {
            gd->mincer_offset = (gd->mincer_offset + 1) % gd->wav->size;
        }
        gd->tbl_pos = (gd->tbl_pos + 1) % gd->wav->size;
    }
    sp->out[0] = out;
    sp->out[1] = out;
    
    sp_rms_compute(sp, gd->rms, &out, &rms);
    sp_port_compute(sp, gd->rms_smooth, &rms, &rms_smooth);
    gd->level = rms_smooth * 4;

    return 0;
}

float auria_cf(crossfade *cf, float v1, float v2)
{
    /* crossfade from v1 to v2 */
    float out = v2;
    if(cf->pos < cf->time) {
        float fade = (float)cf->pos / cf->time;
        out = (fade * v2) + ((1 - fade) * v1);
        cf->pos++;
    }
    return out;
}
int auria_cf_check(crossfade *cf)
{
    return cf->pos < cf->time;
}
