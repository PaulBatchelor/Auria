#include <stdlib.h>
#include "soundpipe.h"
#include "sporth.h"
#include "base.h"

int auria_toggle_duplexf(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;
    auria_data *ad = pd->ud;
    SPFLOAT tick = 0;
    switch(pd->mode) {
        case PLUMBER_INIT:

            fprintf(stderr, "Default user function in init mode.\n");

            sporth_stack_pop_float(stack);
            break;

        case PLUMBER_COMPUTE:
            tick = sporth_stack_pop_float(stack);
            if(tick != 0) {
                auria_toggle_duplex(ad);
            }
            break;
        default:
            break;
    }
    return PLUMBER_OK;
}

int auria_reloadf(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;
    auria_data *ad = pd->ud;
    SPFLOAT tick = 0;
    switch(pd->mode) {
        case PLUMBER_INIT:

            fprintf(stderr, "Default user function in init mode.\n");

            sporth_stack_pop_float(stack);
            break;

        case PLUMBER_COMPUTE:
            tick = sporth_stack_pop_float(stack);
            if(tick != 0) {
                ad->please_recompile = 1;
            }
            break;
        default:
            break;
    }
    return PLUMBER_OK;
}
