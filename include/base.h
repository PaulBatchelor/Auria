#ifdef __cplusplus
extern "C" {
#endif
#ifndef AURIA_MINCER
#include "mincer.h"
#define AURIA_MINCER 1
#endif
enum {
    AURIA_SCROLL,
    AURIA_FREEZE,
    AURIA_PLEASE_FREEZE,
    AURIA_REPLAY,
    AURIA_PLEASE_REPLAY
};

typedef struct  {
    unsigned int time;
    unsigned int pos;
} crossfade;

typedef struct {
    sp_data *sp;
    sp_ftbl *wav;
    auria_mincer *mincer;
    sp_port *portX;
    sp_port *portY;
    sp_port *rms_smooth;
    sp_port *mix;
    sp_rms *rms;
    SPFLOAT level;
    int sr;
    int w, h;
    int state;
    int state_Y;
    float posX, posY;
    float onedsr;
    unsigned int offset;
    float *soundbars;
    float sum;
    unsigned int nbars;
    unsigned int counter;
    unsigned int counter_speed;
    int mode;

    plumber_data pd;
    unsigned int mincer_offset;
    unsigned int wtpos;
    crossfade cf;
}auria_data;

int auria_draw(auria_data *gd);
/* Initializers */
int auria_init(auria_data *gd, char *filename);
int auria_init_audio(auria_data *gd, char *filename);

int auria_compute_audio(auria_data *gd);

/* Destroyers */
int auria_destroy(auria_data *gd);
int auria_destroy_audio(auria_data *gd);

float auria_cf(crossfade *cf, float v1, float v2);
#ifdef __cplusplus
}
#endif
