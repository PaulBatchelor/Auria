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

int auria_draw(auria_data *gd)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

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

    
    /* draw y-ball*/
    int npoints = 256;
    float incr = 2 * M_PI / (npoints - 1);
    float size = 0.1 + 0.3 * gd->level;
    float pY;

    if(gd->state_Y == 1) {
        pY = 2 + fY2 * 2 * (gd->posY) + size * 0.5;
    } else {
        //pY = size * 0.5;
        pY = 0;
    }
    float pX = fX2 * (1 - 2 * gd->posX);
    if(gd->mode == AURIA_FREEZE) {
        //uint32_t index = (uint32_t) floor(gd->posY * gd->dur - 1) % gd->nbars;
        uint32_t index = (gd->line_offset + (uint32_t) floor(gd->posY * gd->dur - 1)) % gd->nbars;
        pX = fX1 * (2 * gd->line[index].x - 1);
        pY = fY2 * (2 * gd->line[index].y - 1);
    } 

   // if(gd->mode == AURIA_FREEZE || gd->mode == AURIA_REPLAY) { 
        glColor3f(0.5607, 0.996, 0.0353);
        
        //glBegin(GL_LINES);
        //glVertex2f(fX2 * (1 - 2 * gd->posX), fY2 );
        //glVertex2f(fX2 * (1 - 2 * gd->posX), fY1);
        //glEnd();

        glBegin(GL_TRIANGLE_FAN);
            for(n = 0; n < npoints; n++) {
                glVertex2f(pX + size * cos(n * incr), 
                        pY + (size * sin(n * incr)));
            }
        glEnd();
    //}

    GLfloat pos1 = 0;
    GLfloat pos2 = 0;
    float amp = 0;
    float w = gd->w;
    float barwidth = w / gd->nbars;
    unsigned int offset = gd->offset;
    float frac = ((float)gd->counter / (gd->counter_speed));
    int index;
    glLineWidth(3.0);
    glColor3f(0, 0.5 , 1);
    glBegin(GL_LINE_STRIP);
    uint32_t dur = gd->dur;
    for(n = 0; n < dur; n++){
        index = (gd->nbars - n + offset - 1) % gd->nbars;
        amp = gd->line[index].amp;
        glColor3f(0, 0.5, 1.0);
        pos1 = 2 * (((n - frac) * barwidth) / w) - 1;
        pos1 = (fX1 * pos1) / fX2;

        pos2 = 2 * (((n + 1 - frac) * barwidth) / w) - 1;
        pos2 = (fX1 * pos2) / fX2;

        //amp = -1 * gd->soundbars[(n + offset) % gd->nbars];
        //glVertex2f(fX2 * pos1, fY2 * amp);
        //glVertex2f(fX2 * pos1, fY1 * amp);
        glVertex2f(fX1 * (2 * gd->line[index].x - 1), 
                fY2 * (2 * gd->line[index].y - 1));
        
        //glVertex2f(fX2 * pos2, fY2 * amp);
        //glVertex2f(fX2 * pos2, fY1 * amp);
        //glVertex2f(fX2 * gd->line[n].x, fY1 * gd->line[n].y);
    }
    glEnd();
    
    glFlush( );
    glutSwapBuffers( );
    return 0;
}
