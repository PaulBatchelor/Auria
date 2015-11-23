typedef struct {
    SPFLOAT time, amp, pitch, lock, iN,
        idecim, onset, offset, dbthresh;
    int cnt, hsize, curframe, N, decim,tscale;
    SPFLOAT pos;
    SPFLOAT accum;
    sp_auxdata outframe, win, bwin, fwin,
    nwin, prev, framecount, indata[2];
    SPFLOAT *tab;
    int curbuf;
    SPFLOAT resamp;
    sp_ftbl *ft;
    sp_fft fft;
    uint32_t wtpos;
    uint32_t size;
} auria_mincer;

int auria_mincer_create(auria_mincer **p);
int auria_mincer_destroy(auria_mincer **p);
int auria_mincer_init(sp_data *sp, auria_mincer *p, sp_ftbl *ft);
int auria_mincer_compute(sp_data *sp, auria_mincer *p, SPFLOAT *in, SPFLOAT *out, uint32_t offset);
