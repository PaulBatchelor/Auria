#ifdef __cplusplus
extern "C" {
#endif
#ifndef AURIA_MINCER
#include "mincer.h"
#define AURIA_MINCER 1
#endif
#include "f310.h"

#define AURIA_BUFSIZE 10

#define AURIA_STACK_SIZE 32

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
    float x, y;
    float amp;
    int draw_circ;
} auria_cor;

typedef struct {
    auria_cor *stack;
    unsigned int pos;
    size_t size; 

    /* FIFO stack operations */
    unsigned int len;
    unsigned int offset;
} auria_stack;

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
    /* total number of bars */
    unsigned int total_bars;
    /* number of bars to draw */
    unsigned int nbars;
    /* duration in bar units */
    unsigned int bar_dur;
    unsigned int counter;
    unsigned int counter_speed;
    int mode;

    plumber_data pd;
    unsigned int mincer_offset;
    unsigned int wtpos;
    crossfade cf;

    f310_d fd;

    /* joystick acceleration */
    float accX;
    float accY;

    /* Flag to Use portamento time. Should almost always be on.*/
    int use_port;

    int run;
    
    /* Line buffer */
    auria_cor *line;
    auria_cor *tmp_line;
    /* length of audio chunk in samples */
    uint32_t size_s;
    uint32_t line_offset;
    uint32_t tbl_pos;

    int drawline;
    int please_draw_circ;

    auria_stack circle_stack;
    auria_stack line_fifo;
    int wrap_mode;
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
int auria_cf_check(crossfade *cf);
void auria_kontrol(int type, int ctl, int val, void *ud);
int auria_switch(auria_data *ad);
int auria_toggle_pitch(auria_data *ad);


/* create/destroy for fifo AND stack */
int auria_stack_create(auria_stack *stack, size_t size);
int auria_stack_destroy(auria_stack *stack);

/* stack operations */
int auria_stack_init(auria_stack *stack);
int auria_stack_push(auria_stack *stack, auria_cor *cor);
unsigned int auria_stack_get_size(auria_stack *stack);
int auria_stack_pop(auria_stack *stack, auria_cor **cor);

/* fifo operations */
int auria_fifo_shift(auria_stack *stack);
int auria_fifo_init(auria_stack *stack);
int auria_fifo_push(auria_stack *stack, auria_cor *cor);
int auria_fifo_return(auria_stack *stack, auria_cor **cor, unsigned int index);
unsigned int auria_fifo_get_len(auria_stack *stack);

#ifdef __cplusplus
}
#endif
