#include <stdint.h>
#include <stdlib.h>
#include "soundpipe.h"
#include "sporth.h"
#include "base.h"

int auria_stack_init(auria_stack *stack)
{
    stack->pos = 0;
    return 1;
}
int auria_stack_pop(auria_stack *stack, auria_cor **cor)
{
    if(stack->pos == 0) {
        fprintf(stderr, "Warning: empty stack. \n");
        return 1;
    }

    stack->pos--;
    *cor = &stack->stack[stack->pos];
    return 0;
}

unsigned int auria_stack_get_size(auria_stack *stack)
{
    return stack->pos;
}

int auria_stack_push(auria_stack *stack, auria_cor *cor)
{
    if(stack->pos >= AURIA_STACK_SIZE) {
        //fprintf(stderr, "warning: stack is full.\n");
        return 1;
    }

    /* Current position == next index because 1-index */
    unsigned int pos =  stack->pos;
    stack->pos++;
    stack->stack[pos].x = cor->x;
    stack->stack[pos].y = cor->y;
    stack->stack[pos].draw_circ = cor->draw_circ;
    stack->stack[pos].amp = cor->amp;

    return 0;
}

int auria_stack_create(auria_stack *stack, size_t size)
{
    stack->stack = malloc(sizeof(auria_cor) * size);
    return 0;
}

int auria_stack_destroy(auria_stack *stack)
{
    free(stack->stack);
    return 0;
}
