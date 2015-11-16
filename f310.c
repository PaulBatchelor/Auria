#include <stdio.h>
#include <pthread.h>
#include "f310.h"

static void *listen(void *ud) 
{
    f310_d *fd = ud;
    FILE *fp = fopen("/dev/input/js0", "r");

    if(fp == NULL) {
        printf("There was a problem reading the F310 controller. Exiting gracefully...\n");
        return;
    }

    signed char byte[8], type, ctl;
    int val;
    int i;

    while(1){
        fread(byte, 1, 8, fp);
        fd->foo++;
        val  = byte[4] + (byte[5] << 8);
        type = byte[6]; 
        ctl = byte[7];

        if((~type == 0x007e) || (~type == 0x007d)) {
            /* initialization, no need to print to screen */
        } else {
            fd->func(type, ctl, val, fd->ud);
        }
    }
    fclose(fp);
    pthread_exit(NULL);
}

int f310_start(f310_d *fd, void (* func)(int, int, int, void *), void *ud)
{
    long t = 0;
    int rc;
    fd->run = 1;
    fd->func = func;
    fd->ud = ud;
    rc = pthread_create(&fd->thread, NULL, listen, fd);
    return 0;
}

int f310_stop(f310_d *fd)
{
    fd->run = 0;
    return 0;
}

/*
int main() {
    f310_d fd;
    fd.foo = 0;
    f310_start(&fd, run);
    int i = 5;
    while(i > 0 ) {
        printf("%d \n", i);
        sleep(1);
        i--;
    }
    f310_stop(&fd);
    return 0;
}
*/
