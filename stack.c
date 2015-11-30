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
    if(stack->pos >= stack->size) {
        //fprintf(stderr, "warning: stack is full.\n");
        return 1;
    }

    /* Current position == next index because 1-index */
    unsigned int pos =  stack->pos;
    stack->pos++;
    stack->stack[pos].pt.x = cor->pt.x;
    stack->stack[pos].pt.y = cor->pt.y;
    stack->stack[pos].draw_circ = cor->draw_circ;
    stack->stack[pos].amp = cor->amp;

    return 0;
}

int auria_stack_create(auria_stack *stack, size_t size)
{
    stack->stack = malloc(sizeof(auria_cor) * size);
    stack->pos = 0;
    stack->size = size;
    stack->offset = 0;
    stack->len = 0;
    return 0;
}

int auria_stack_destroy(auria_stack *stack)
{
    free(stack->stack);
    return 0;
}

int auria_fifo_push(auria_stack *stack, auria_cor *cor)
{

    if(stack->len >= stack->size) {
        fprintf(stderr, "Warning: stack overflow\n");
        return 0;
    }
    unsigned int wrapped_index = (stack->offset + stack->len) % stack->size;
    stack->len++;
    
    stack->stack[wrapped_index].pt.x = cor->pt.x;
    stack->stack[wrapped_index].pt.y = cor->pt.y;
    stack->stack[wrapped_index].amp = cor->amp;
    stack->stack[wrapped_index].draw_circ= cor->draw_circ;

    return 0;
}

int auria_fifo_return(auria_stack *stack, auria_cor **cor, unsigned int index)
{
    unsigned int wrapped_index = (stack->offset + index) % stack->size;
    *cor = &stack->stack[wrapped_index];
    return 0;
}

unsigned int auria_fifo_get_len(auria_stack *stack)
{
    return stack->len;
}
int auria_fifo_shift(auria_stack *stack)
{
    stack->offset = (stack->offset + 1) % stack->size;
    /* found this was needed */
    stack->len--;
    return 0;
}
int auria_fifo_init(auria_stack *stack)
{
    stack->offset = 0;
    stack->len = 0;
    return 0;
}
