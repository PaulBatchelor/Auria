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
    float size = 0.3 * gd->level;
    float pY;
    if(gd->state_Y == 1) {
        pY = 2 + fY2 * 2 * (gd->posY) + size * 0.5;
    } else {
        //pY = size * 0.5;
        pY = 0;
    }
    float pX = fX2 * (1 - 2 * gd->posX);
   
    if(gd->pause == 1) { 
        glColor3f(0.5607, 0.996, 0.0353);
        
        glBegin(GL_LINES);
        glVertex2f(fX2 * (1 - 2 * gd->posX), fY2 );
        glVertex2f(fX2 * (1 - 2 * gd->posX), fY1);
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
            for(n = 0; n < npoints; n++) {
                glVertex2f(pX + size * cos(n * incr), 
                        pY + (size * sin(n * incr)));
            }
        glEnd();
    }

    glColor3f(0, 0.5, 1);
    GLfloat pos1 = 0;
    GLfloat pos2 = 0;
    float amp = 0;
    float w = gd->w;
    float barwidth = w / gd->nbars;
    unsigned int offset = gd->offset;
	unsigned int counter = gd->counter;
    float frac = (float)(barwidth * gd->counter / gd->counter_speed);
	unsigned int so = gd->screen_offset;
	
    for(n = 0; n < gd->nbars; n++){
		offset = ((n * (int)barwidth) + so) % gd->w;
        pos1 = 2 * (offset/ w) - 1;
        pos1 = (fX1 * pos1) / fX2;

		offset = (((n + 1) * (int)barwidth) + so) % gd->w;
        pos2 = 2 * (so / w) - 1;
        pos2 = (fX1 * pos2) / fX2;

        glBegin(GL_TRIANGLE_STRIP);
        //amp = -1 * gd->soundbars[(n + offset) % gd->nbars];
        amp = -1 * gd->soundbars[n];
        glVertex2f(fX2 * pos1, fY2 * amp);
        glVertex2f(fX2 * pos1, fY1 * amp);
        
        glVertex2f(fX2 * pos2, fY2 * amp);
        glVertex2f(fX2 * pos2, fY1 * amp);
        glEnd();
    }
    
    glFlush( );
    glutSwapBuffers( );
    return 0;
}
