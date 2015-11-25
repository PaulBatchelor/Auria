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

int auria_draw(auria_data *gd)
{

    int n;

    uint32_t skip = gd->wav->size / gd->w;
    GLfloat depth = 0;
    GLdouble fX1, fY1, fZ1;
    GLdouble fX2, fY2, fZ2;
    GLdouble  model[16], proj[16];
    GLint view[4];
    
    glReadPixels(gd->w, gd->h, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glMatrixMode (GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);
    gluUnProject(gd->w, gd->h, 0.8182, model, proj, view, &fX1, &fY1, &fZ1);
    
    glReadPixels(0, 0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glMatrixMode (GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);
    gluUnProject(0, 0, 0.8182, model, proj, view, &fX2, &fY2, &fZ2);

    
    int npoints = 256;
    float incr = 2 * M_PI / (npoints - 1);
    float size = 0.1 + 0.3 * gd->level;
    float pY;

    if(gd->state_Y == 1) {
        pY = 2 + fY2 * 2 * (gd->posY) + size * 0.5;
    } else {
        pY = 0;
    }
    float pX = fX2 * (1 - 2 * gd->posX);
    if(gd->mode == AURIA_FREEZE) {
        uint32_t index = (gd->line_offset - 1 + (uint32_t) floor(gd->posY * (gd->dur- 1))) % (gd->nbars);
        pX = fX1 * (2 * gd->line[index].x - 1);
        pY = fY2 * (2 * gd->line[index].y - 1);
        glClearColor(
                1 * gd->line[index].amp, 
                1 * gd->line[index].amp, 
                1 * gd->line[index].amp, 
                1);
    } else {
        glClearColor(1 * gd->pd.p[2], 1 * gd->pd.p[2], 1 * gd->pd.p[2], 1 );
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Draw the ball */

    glColor3f(0.5607, 0.996, 0.0353);
    
    glBegin(GL_TRIANGLE_FAN);
        for(n = 0; n < npoints; n++) {
            glVertex2f(pX + size * cos(n * incr), 
                    pY + (size * sin(n * incr)));
        }
    glEnd();

    GLfloat pos1 = 0;
    GLfloat pos2 = 0;
    float w = gd->w;
    float barwidth = w / gd->nbars;
    unsigned int offset = gd->offset;
    int index;
    glLineWidth(3.0);
    glColor3f(0, 0.5 , 1);
    uint32_t dur = gd->dur;
    glBegin(GL_LINE_STRIP);
    float x, y;
    for(n = 0; n < dur; n++){
        index = (gd->nbars - n + offset - 1) % gd->nbars;
        x = gd->line[index].x;
        y = gd->line[index].y;
        pos1 = 2 * (((n) * barwidth) / w) - 1;
        pos1 = (fX1 * pos1) / fX2;
        pos2 = 2 * (((n + 1) * barwidth) / w) - 1;
        pos2 = (fX1 * pos2) / fX2;
        
        glVertex2f(fX1 * (2 * x - 1), 
                fY2 * (2 * y - 1));
    }
    glEnd();
    
    if(gd->drawline == 1) {
        auria_cor *cor = &gd->line[gd->offset];
        cor->x = gd->posX;
        cor->y = gd->posY;
        cor->amp = gd->pd.p[2];
        gd->dur++;
        gd->dur = min(gd->dur, gd->nbars);
        gd->offset = (gd->offset + 1) % gd->nbars;
        gd->drawline = 0;
        if(gd->dur >= gd->nbars) {
            gd->line_offset = (gd->line_offset + 1) % (gd->nbars);
        }
    }
    
    glutSwapBuffers( );
    glFlush( );
    return 0;
}
