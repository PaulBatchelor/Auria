#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "soundpipe.h"
#include "sporth.h"
#include "base.h"

static float joy_to_float(int val, float acc);

static int hold(auria_data *ad, int val);


void auria_kontrol(int type, int ctl, int val, void *ud)
{
    //if(type == 1) {
    //    switch(ctl) {
    //        case BUT_A: printf("but_a %d", val); break;
    //        case BUT_B: printf("but_b %d", val); break;
    //        case BUT_X: printf("but_x %d", val); break;
    //        case BUT_Y: printf("but_y %d", val); break;
    //        case BUT_LB: printf("but_lb"); break;
    //        case BUT_RB: printf("but_rb"); break;
    //        case BUT_BACK: printf("but_back"); break;
    //        case BUT_START: printf("but_start"); break;
    //        case BUT_JOY_L: printf("but_joy_l"); break;
    //        case BUT_JOY_R: printf("but_joy_r"); break;
    //        case BUT_LOG: printf("but_log"); break;
    //    }
    //}else if(type == 2) {
    //        switch(ctl) {
    //        case JOY_L_X: printf("joy_l_x"); break;
    //        case JOY_L_Y: printf("joy_l_y"); break;
    //        case TRIG_L: printf("trig_l"); break;
    //        case JOY_R_X: printf("joy_r_x"); break;
    //        case JOY_R_Y: printf("joy_r_y"); break;
    //        case TRIG_R: printf("trig_r"); break;
    //        case DPAD_H: printf("dpad_h"); break;
    //        case DPAD_V: printf("dpad_v"); break;
    //    }
    //}
    
    auria_data *ad = ud;
    if(type == 1) {
        switch(ctl) {
            case BUT_A: 
                printf("but_a %d\n", val); 
                if(val == 1) auria_switch(ad);
                break;
            case BUT_B: 
                printf("but_b %d\n", val); 
                //if(val == 1) auria_toggle_pitch(ad);
                break;
            case BUT_RB:
                printf("but_rb %d\n", val); 
                hold(ad, val);
                break;
            case BUT_LB:
                printf("but_lb %d\n", val); 
                if(val == 1) {
                    ad->please_trig_lb = 1;
                }
                break;
            default: break;
        }
    } else if(type == 2) {
        switch(ctl) {
            case JOY_L_X: 
                ad->accX = joy_to_float(val, 0.000015);
                break;
            case JOY_L_Y: 
                ad->accY = joy_to_float(val, 0.000015);
                if(ad->mode == AURIA_FREEZE) {
                    ad->accY = -joy_to_float(val, 0.000015);
                }
                break;
            case JOY_R_Y: 
                //if(ad->mode == AURIA_SCROLL) {
                    ad->accZ = joy_to_float(val, 0.00015);
                //fprintf(stderr, "%d %g\n", val, ad->accZ);
                //}
                break;
            default: break;
        }

    }
}
int auria_switch(auria_data *ad)
{
    if(ad->mode == AURIA_REPLAY) {
        ad->mode = AURIA_PLEASE_FREEZE;
        ad->cf.pos = 0;
    } else if (ad->mode == AURIA_SCROLL) {
        ad->mode = AURIA_PLEASE_FREEZE;
        ad->mincer->size = ad->size_s;
        ad->posY = 1;
        ad->drawline = 0;
    } else if(ad->mode == AURIA_FREEZE) {
        //ad->mode = AURIA_PLEASE_REPLAY;
        ad->mode = AURIA_SCROLL;
        uint32_t index = (uint32_t) (ad->line_offset + floor(ad->posY * ad->nbars- 1)) % ad->total_bars;
        ad->posX = ad->line[index].pt.x;
        ad->posY = ad->line[index].pt.y;
        ad->cf.pos = 0;
        ad->nbars = 0;
        ad->offset = 0;
        ad->size_s = 0;
        ad->line_offset = 0;
        ad->mincer_offset = 0;
        ad->tbl_pos = 0;
        ad->counter = 0;
        ad->drawline = 0;
        ad->wrap_mode = 0;
        /* reset line fifo */
        auria_fifo_init(&ad->line_fifo);
        /* reset ghosting stack */
        ad->ghosts.last = 0;
        ad->ghosts.pos = 0;
        ad->ghosts.len = 0;
    }
    return 0;
}

int auria_toggle_pitch(auria_data *ad) {
    ad->state_Y = (ad->state_Y == 1) ? 0 : 1 ;
    ad->posY = 0.5;
    return 0;
}

static float joy_to_float(int val, float acc) 
{
    if(abs(val) > 1050) {
        float speed = (2 * ((float)(val + 32767) / 65278) - 1);
        /*TODO make this constant based on samplerate */
        //return speed * 0.000015;
        return speed * acc;
    } else {
        return 0;
    }
}

static int hold(auria_data *ad, int val)
{
    if(val == 1) {
        ad->hold_y = ad->accY;
        ad->hold_x = ad->accX;
        ad->accY = 0;
        ad->accX = 0;
    } else {
        ad->hold_y = 0;
        ad->hold_x = 0;
    }
    return 0;
}
