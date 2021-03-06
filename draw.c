#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#endif
#include <soundpipe.h>
#include <sporth.h>
#include "math.h"
#include "base.h"

#define min(a, b) (a < b) ? a : b

static int draw_ticks(auria_data *gd, float fX, float fY) 
{
    uint32_t n;
    auria_cor *cor;
    float x1, x2, y1, y2, z; 
    float size;
    unsigned int stack_size = (uint32_t)auria_stack_get_size(&gd->circle_stack);

    for(n = 0; n < stack_size; n++) {
        cor = NULL;
        auria_stack_pop(&gd->circle_stack, &cor);

        if(cor == NULL) {
            fprintf(stderr, "ERROR!\n");
        }

        size = 0.05;

        x1 = fX * (2 * cor->pt.x - 1) - size;
        x2 = fX * (2 * cor->pt.x - 1) + size;

        y1 = fY * (2 * cor->pt.y - 1) + size;
        y2 = fY * (2 * cor->pt.y - 1) - size;
        z = cor->pt.z;       
 
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(x1, y1, z);
            glVertex3f(x1, y2, z);
            glVertex3f(x2, y2, z);
            glVertex3f(x2, y1, z);
        glEnd();
    }

    return 0;
}

static int get_projection_coefs(auria_data *gd, 
        GLdouble *fX1, GLdouble *fX2,
        GLdouble *fY1, GLdouble *fY2,
        GLdouble *fZ1, GLdouble *fZ2
        )
{
    GLdouble  model[16], proj[16];
    GLint view[4];
    GLfloat depth = 0;

    glReadPixels(gd->w, gd->h, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glMatrixMode (GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);
    gluUnProject(gd->w, gd->h, 0.8182, model, proj, view, fX1, fY1, fZ1);
    
    glReadPixels(0, 0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glMatrixMode (GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);
    gluUnProject(0, 0, 0.8182, model, proj, view, fX2, fY2, fZ2);

    return 0;
}

static int draw_line(auria_data *gd, GLdouble fX, GLdouble fY) 
{
    unsigned int offset = gd->offset;
    uint32_t n;
    int index;
    glLineWidth(3.0);
    glColor3f(0, 0.5 , 1);
    uint32_t nbars = gd->nbars;

    glBegin(GL_LINE_STRIP);
    float x, y;
    auria_cor *cor = NULL;
    auria_stack_init(&gd->circle_stack);
    for(n = 0; n < nbars; n++){
        index = (gd->total_bars - n + offset - 1) % gd->total_bars;
        cor = &gd->line[index];
        x = cor->pt.x;
        y = cor->pt.y;

        if(gd->line[index].draw_circ == 1) {
            auria_stack_push(&gd->circle_stack, cor);
        }

        glVertex2f(fX * (2 * x - 1), 
                fY * (2 * y - 1));


    }
    glEnd();
    return 0;
}

/* FIFO draw line */

static int draw_line2(auria_data *gd, GLdouble fX, GLdouble fY) 
{
    unsigned int offset = gd->offset;
    uint32_t n;
    int index;
    glLineWidth(3.0);
    glColor3f(0, 0.5 , 1);
    uint32_t len = auria_fifo_get_len(&gd->line_fifo);

    glBegin(GL_LINE_STRIP);
    float x, y, z;
    auria_cor *cor = NULL;
    auria_stack_init(&gd->circle_stack);
    for(n = 0; n < len; n++){
        auria_fifo_return(&gd->line_fifo, &cor, n);

        x = cor->pt.x;
        y = cor->pt.y;
        z = cor->pt.z;
        
        if(cor->draw_circ == 1) {
            auria_stack_push(&gd->circle_stack, cor);
        }

        glVertex3f(fX * (2 * x - 1), 
                fY * (2 * y - 1), z);
    }
    glEnd();
    return 0;
}

static void draw_ball(auria_data *gd, 
        float size, float pX, float pY, float pZ,
        GLdouble fX, GLdouble fY) 
{
    uint32_t n, circ, index;

    int npoints = 256;
    float incr = 2 * M_PI / (npoints - 1);
    float eps = 0.000001;

    if(gd->mode == AURIA_FREEZE) {
        int pos = gd->ghosts.pos;    
        gd->ghosts.pos = (pos + 1) % AURIA_NUM_TRAILS;
        gd->ghosts.pt[pos].x = pX;
        gd->ghosts.pt[pos].y = pY;
        gd->ghosts.pt[pos].z = pZ;
        gd->ghosts.size[pos] = size;
        gd->ghosts.color[pos].r = gd->ball_color.r;
        gd->ghosts.color[pos].g = gd->ball_color.g;
        gd->ghosts.color[pos].b = gd->ball_color.b;
        gd->ghosts.color[pos].a = 1;
        gd->ghosts.len = min(AURIA_NUM_TRAILS, gd->ghosts.len + 1);
        gd->ghosts.last = pos;

        index = gd->ghosts.last;
        float opacity = 1;
        for(circ = 0; circ < gd->ghosts.len; circ++) {
            auria_glcolor(&gd->ghosts.color[index]);
            glBegin(GL_TRIANGLE_FAN);
                for(n = 0; n < npoints; n++) {
                    glVertex3f(gd->ghosts.pt[index].x + size * cos(n * incr), 
                            gd->ghosts.pt[index].y + (size * sin(n * incr)),
                            gd->ghosts.pt[index].z);
                }
            glEnd();
            index = (AURIA_NUM_TRAILS + index - 1) % AURIA_NUM_TRAILS;
            opacity *= 0.6;
        }

    } else {
        auria_glcolor(&gd->ball_color); 
        glBegin(GL_TRIANGLE_FAN);
            for(n = 0; n < npoints; n++) {
                glVertex3f(pX + size * cos(n * incr), 
                        pY + (size * sin(n * incr)), pZ);
            }
        glEnd();
    }
}

static int add_new_point2(auria_data *gd)
{
    if(gd->mode == AURIA_SCROLL) {
        auria_cor cor;

        int please_draw_circ = gd->please_draw_circ;
        cor.pt.x = gd->posX;
        cor.pt.y = gd->posY;
        cor.pt.z = gd->posZ;
        cor.draw_circ = 0;

        cor.ball_color.r = gd->ball_color.r;
        cor.ball_color.g = gd->ball_color.g;
        cor.ball_color.b = gd->ball_color.b;
        
        cor.bgcolor.r = gd->bgcolor.r;
        cor.bgcolor.g = gd->bgcolor.g;
        cor.bgcolor.b = gd->bgcolor.b;

        if(please_draw_circ == 1) {
            cor.draw_circ = 1;
            gd->please_draw_circ = 0;
        }

        if(gd->wrap_mode) {
            auria_fifo_shift(&gd->line_fifo);
        }

        auria_fifo_push(&gd->line_fifo, &cor);
    }

    return 0;
}

static int add_new_point(auria_data *gd)
{
    auria_cor *cor;

    if(gd->drawline == 1) {
        int please_draw_circ = gd->please_draw_circ;
        cor = &gd->line[gd->offset];
        cor->pt.x = gd->posX;
        cor->pt.y = gd->posY;
        cor->amp = gd->pd.p[2];
        cor->draw_circ = 0;
        if(please_draw_circ == 1) {
            cor->draw_circ = 1;
            gd->please_draw_circ = 0;
        }
        gd->nbars++;
        gd->nbars = min(gd->nbars, gd->total_bars);
        gd->offset = (gd->offset + 1) % gd->total_bars;
        gd->drawline = 0;
        if(gd->nbars >= gd->total_bars) {
            gd->line_offset = (gd->line_offset + 1) % (gd->total_bars);
        }

    }
    return 0;
}

static int get_position(auria_data *gd, uint32_t index, float *pX, float *pY,
        GLdouble fX, GLdouble fY)
{
    *pX = fX * (2 * gd->line[index].pt.x - 1);
    *pY = fY * (2 * gd->line[index].pt.y - 1);
    return 0;
}

static int get_position2(auria_data *gd, uint32_t index, float *pX, float *pY, float *pZ,
        GLdouble fX, GLdouble fY)
{
    auria_cor *cor=NULL;
    auria_fifo_return(&gd->line_fifo, &cor, index);
    *pX = fX * (2 * cor->pt.x - 1);
    *pY = fY * (2 * cor->pt.y - 1);
    *pZ = cor->pt.z;
    return 0;
}

int auria_draw(auria_data *gd)
{
    glEnable( GL_BLEND );
    GLdouble fX1, fY1, fZ1;
    GLdouble fX2, fY2, fZ2;
    
    get_projection_coefs(gd, &fX1, &fX2, &fY1, &fY2, &fZ1, &fZ2);
    
    float pY = 0;
    float level = gd->level;

    if(isinf(level)){
        level = 0;
    }
    float size = 0.1 + 0.3 * level;
    if(gd->state_Y == 1) {
        pY = 2 + fY2 * 2 * (gd->posY) + size * 0.5;
    } else {
        pY = 0;
    }
    
    float pX = 0;
    pX = fX2 * (1 - 2 * gd->posX);
    float pZ = gd->posZ;   
 
    if(gd->mode == AURIA_FREEZE) {
        uint32_t index = floor(gd->posY * (auria_fifo_get_len(&gd->line_fifo) - 1));
        get_position2(gd, index, &pX, &pY, &pZ, fX1, fY2);
        auria_cor *cor = NULL;
        auria_fifo_return(&gd->line_fifo, &cor, index);
        auria_set_color(&gd->ball_color, cor->ball_color.r, cor->ball_color.g, cor->ball_color.b); 
        glClearColor(
                cor->bgcolor.r, 
                cor->bgcolor.g, 
                cor->bgcolor.b, 
                1);
    } else {
        glClearColor(gd->bgcolor.r, gd->bgcolor.g, gd->bgcolor.b, 1 );
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    
    if(gd->rot_X > 0) {
        glRotated(1, 0, 1, 0);
        gd->rot_X_off++;
    } else if(gd->rot_X < 0) {
        glRotated(-1, 0, 1, 0);
        gd->rot_X_off--;
    }


    draw_ball(gd, size, pX, pY, pZ, 10 * fX2, 10 * fY2);
    draw_line2(gd, fX1, fY2);  
    draw_ticks(gd, fX1, fY2);

    add_new_point2(gd); 

    glutSwapBuffers( );
    glFlush( );
    return 0;
}

int auria_rotation_reset(auria_data *ad)
{
    glRotated(-ad->rot_X_off, 0, 1, 0);
    ad->rot_X_off = 0;
}

