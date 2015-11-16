#ifndef F310_H
enum BUTTONS {
    BUT_A,
    BUT_B,
    BUT_X,
    BUT_Y,
    BUT_LB,
    BUT_RB,
    BUT_BACK,
    BUT_START,
    BUT_LOG,
    BUT_JOY_L = 9,
    BUT_JOY_R,
    JOY_L_X = 0, 
    JOY_L_Y,
    TRIG_L,
    JOY_R_X,
    JOY_R_Y,
    TRIG_R,
    DPAD_H,
    DPAD_V
};
typedef struct {
    void (*func)(int type, int ctl, int val, void *);
    int run;
    int foo;
    pthread_t thread;
    void *ud;
} f310_d;

int f310_start(f310_d *fd, void (* func)(int, int, int, void *), void *ud);
int f310_stop(f310_d *fd);
#define F310_H 1
#endif
