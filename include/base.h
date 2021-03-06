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
#define AURIA_NUM_TRAILS 10 
#define AURIA_ARG_SIZE 20

#define AUDIO_INPUT arg_tbl->tbl[0]
#define BALL_X_POS arg_tbl->tbl[1]
#define BALL_Y_POS arg_tbl->tbl[2]
#define BALL_Z_POS arg_tbl->tbl[3]


#define JS_L_X arg_tbl->tbl[4]
#define JS_L_Y arg_tbl->tbl[5]
#define JS_R_X arg_tbl->tbl[6]
#define JS_R_Y arg_tbl->tbl[7]

#define BALL_R arg_tbl->tbl[8]
#define BALL_G arg_tbl->tbl[9]
#define BALL_B arg_tbl->tbl[10]

#define BGCOLOR_R arg_tbl->tbl[11]
#define BGCOLOR_G arg_tbl->tbl[12]
#define BGCOLOR_B arg_tbl->tbl[13]

#define LB_TICK arg_tbl->tbl[14]
#define DRAW_TICK arg_tbl->tbl[15]

#define MINCER_OUT arg_tbl->tbl[16]

#define BUT_BLUE arg_tbl->tbl[17]
#define BUT_YELLOW arg_tbl->tbl[18]
#define BUT_RED arg_tbl->tbl[19]

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
    float x, y, z;
} auria_point;

typedef struct {
    float r, g, b, a;
} auria_color;

typedef struct {
    auria_point pt;
    float amp;
    int draw_circ;
    auria_color ball_color;
    auria_color bgcolor;
} auria_cor;

typedef struct {
    int counter;
    time_t ltime;
} live_coding;

typedef struct {
    auria_cor *stack;
    unsigned int pos;
    size_t size; 

    /* FIFO stack operations */
    unsigned int len;
    unsigned int offset;
} auria_stack;


typedef struct {
    auria_point pt[AURIA_NUM_TRAILS];
    float size[AURIA_NUM_TRAILS];
    auria_color color[AURIA_NUM_TRAILS];
    int pos, len, last;
} auria_circ_stack;


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
    float posX, posY, posZ;
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

    /* joystick params */
    float js_L_X;
    float js_L_Y;
    float js_R_Y;
    /* joystick acceleration */
    float accX;
    float accY;
    float accZ;

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

    /*TODO: rename to notch stack */
    auria_stack circle_stack;
    auria_stack line_fifo;
    int wrap_mode;

    /* ghost trail for circle */

    auria_circ_stack ghosts;

    float hold_y;
    float hold_x;

    int please_trig_lb;

    int rot_X;
    float rot_X_off;
    int rot_Y;
    float rot_Y_off;

    /* live coding for sporth */

    live_coding lc;

    int please_recompile;


    /* argument table, accessible from Sporth as 'aur' */

    sp_ftbl *arg_tbl;

    /* ball color */
    auria_color ball_color;
    auria_color bgcolor;

    /* duplex mode */
    int duplex_mode;

    /* aux buttons */

    int please_tick_blue;
    int please_tick_yellow;
    int please_tick_red;

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

/* rotation reset */

int auria_rotation_reset(auria_data *ad);

/* set color */

int auria_set_color(auria_color *c, float r, float g, float b);
int auria_set_opacity(auria_color *c, float a);
int auria_glcolor(auria_color *c);

/* duplex mode */

int auria_toggle_duplex(auria_data *ad);

/* user accessible functions in Sporth */

int auria_reload(auria_data *ad);
int auria_toggle_duplexf(sporth_stack *stack, void *ud);
int auria_reloadf(sporth_stack *stack, void *ud);

#ifdef __cplusplus
}
#endif
