#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    sp_data *sp;
    sp_ftbl *wav;
    sp_mincer *mincer;
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
    unsigned int nbars;
    unsigned int counter;
    unsigned int counter_speed;
	unsigned int screen_offset;
    int pause;

    plumber_data pd;
    unsigned int mincer_offset;
}auria_data;

int auria_draw(auria_data *gd);
/* Initializers */
int auria_init(auria_data *gd, char *filename);
int auria_init_audio(auria_data *gd, char *filename);

int auria_compute_audio(auria_data *gd);

/* Destroyers */
int auria_destroy(auria_data *gd);
int auria_destroy_audio(auria_data *gd);
#ifdef __cplusplus
}
#endif
