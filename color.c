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

int auria_set_color(auria_color *c, float r, float g, float b)
{
    c->r = r;
    c->g = g;
    c->b = b;
    return 0;
}

int auria_set_opacity(auria_color *c, float a) 
{
    c->a = a;
    return 0;
}

int auria_glcolor(auria_color *c)
{
    glColor4f(c->r, c->g , c->b, c->a);
    return 0;
}
