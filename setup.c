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
    gd->posX = 0.5;
    gd->posZ = 0;
    gd->level = 0;
    gd->offset = 0;
    gd->total_bars = 500;
    gd->mode = AURIA_FREEZE;

    auria_init_audio(gd, filename);

    gd->pd.ud = gd;
    gd->pd.f[0] = auria_toggle_duplexf;
    gd->pd.f[1] = auria_reloadf;
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
    gd->accZ = 0;

    gd->run = 1;

    /* init linebuf TODO: remove linebuf*/
    gd->line = (auria_cor *)malloc(sizeof(auria_cor) * gd->total_bars);
    gd->tmp_line = (auria_cor *)malloc(sizeof(auria_cor) * gd->total_bars);

    for(n = 0; n < gd->total_bars; n++) {
        gd->line[n].pt.x = 0;
        gd->line[n].pt.y = 0;
        gd->line[n].pt.z = 0;
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
    gd->wrap_mode = 0;

    /*circle stack setup */

    gd->ghosts.pos = 0;
    gd->ghosts.len = 0;
    gd->ghosts.last = 0;
    
    for(n = 0; n < AURIA_NUM_TRAILS; n++) {
        gd->ghosts.pt[n].x = 0.5;
        gd->ghosts.pt[n].y = 0.5;
    }

    gd->hold_y = 0;
    gd->hold_x = 0;
    gd->please_trig_lb = 0;

    gd->rot_X = 0;

    gd->js_L_Y = 0;
    gd->js_L_X = 0;
    gd->js_R_Y = 0;

    gd->lc.counter = 0;
    gd->lc.counter = 0;

    gd->rot_X_off = 0;
    gd->arg_tbl->tbl[0] = 0.5; 

    auria_set_color(&gd->ball_color, 0.5607, 0.996, 0.0353);
    auria_set_opacity(&gd->ball_color, 1);
    
    auria_set_color(&gd->bgcolor, 1, 1, 1);
    auria_set_opacity(&gd->bgcolor, 1);

    /* set auria argument color defaults */

    gd->BALL_R = gd->ball_color.r;
    gd->BALL_G = gd->ball_color.g;
    gd->BALL_B = gd->ball_color.b;
    
    gd->BGCOLOR_R = gd->bgcolor.r;
    gd->BGCOLOR_G = gd->bgcolor.g;
    gd->BGCOLOR_B = gd->bgcolor.b;


    gd->duplex_mode = 0;
    gd->please_tick_red = 0; 
    gd->please_tick_yellow = 0; 
    gd->please_tick_blue = 0; 

    gd->please_recompile = 0;
    return 0; 
}
