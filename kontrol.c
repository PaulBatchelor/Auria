#include <stdio.h>
#include "soundpipe.h"
#include "sporth.h"
#include "base.h"

static float joy_to_float(int val);

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
                auria_switch(ad);
                break;
            default: break;
        }
    } else if(type == 2) {
        switch(ctl) {
            case JOY_L_X: 
                {
                //printf("x: %g %d\n", (float)(val + 32767) / 65278, val);
                //if(abs(val) > 1050) {
                //    float speed = (2 * ((float)(val + 32767) / 65278) - 1);
                //    /*TODO make this constant based on samplerate */
                //    ad->accX = speed * 0.00001;
                //} else {
                //    ad->accX = 0;
                //}
                ad->accX = joy_to_float(val);
                break;
                }
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
        ad->cf.pos = 0;
        /* possible way to fix clicking */
        if(ad->posX == 1) {
         ad->posX = (float)(ad->wav->size - 1.5 * ad->cf.time) / ad->wav->size;
        }
    } else if(ad->mode == AURIA_FREEZE) {
        ad->mode = AURIA_PLEASE_REPLAY;
        ad->cf.pos = 0;
        //g_data.wtpos = g_data.mincer->wtpos;
    }
}

static float joy_to_float(int val) 
{
    if(abs(val) > 1050) {
        float speed = (2 * ((float)(val + 32767) / 65278) - 1);
        /*TODO make this constant based on samplerate */
        return speed * 0.00001;
    } else {
        return 0;
    }
}
