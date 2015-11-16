#include <math.h>
#include <stdlib.h>
#include <iostream>

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

#include "RtAudio.h"

extern "C" {
#include "soundpipe.h"
#include "sporth.h"
}

#include "base.h"

using namespace std;

// our datetype
#define SAMPLE float
// corresponding format for RtAudio
#define MY_FORMAT RTAUDIO_FLOAT32
// sample rate
#ifndef MY_SRATE
#define MY_SRATE 48000
#endif
// number of channels
#define MY_CHANNELS 2
// for convenience
#define MY_PIE 3.14159265358979
#define BUFSIZE 1024
#define FFTSIZE 1024
    
auria_data g_data;
RtAudio audio;

long g_width = 640;
long g_height = 480;

void mouseFunc( int button, int state, int x, int y );
void passiveMotionFunc(int x, int y);
void initGfx();
void idleFunc();

int callme( void * outputBuffer, void * inputBuffer, unsigned int numFrames,
            double streamTime, RtAudioStreamStatus status, void * data )
{
    auria_data *gd = (auria_data *) data;

    SAMPLE * input = (SAMPLE *)inputBuffer;
    SAMPLE * output = (SAMPLE *)outputBuffer;

    for( int i = 0; i < numFrames * MY_CHANNELS; i+=2 )
    {
        auria_compute_audio(gd);
        output[i] = gd->sp->out[0];
        output[i + 1] = gd->sp->out[1];
    }
    
    return 0;
}

void displayFunc( )
{
    auria_draw(&g_data);
}

static void stop_audio() 
{
    if( audio.isStreamOpen() )
    {
        audio.stopStream();
        audio.closeStream();
    }

}

int main( int argc, char ** argv )
{
    unsigned int bufferFrames = BUFSIZE;
   
    if(argc < 2) {
        printf("not enough args\n");
        exit(1);
    }

    if( audio.getDeviceCount() < 1 )
    {
        cout << "no audio devices found!" << endl;
        exit( 1 );
    }
    
    /* Init */
  
    auria_init(&g_data, argv[1]); 

    /* initialize GLUT */
    glutInit( &argc, argv );
    /* init gfx */
    initGfx();
    
    audio.showWarnings( true );
    
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = audio.getDefaultInputDevice();
    iParams.nChannels = 1;
    iParams.firstChannel = 0;
    oParams.deviceId = audio.getDefaultOutputDevice();
    oParams.nChannels = MY_CHANNELS;
    oParams.firstChannel = 0;
    
    RtAudio::StreamOptions options;
    
   
    try {
        //audio.openStream( &oParams, &iParams, MY_FORMAT, MY_SRATE, &bufferFrames, &callme, &g_data, &options );
        audio.openStream( &oParams, NULL, MY_FORMAT, MY_SRATE, &bufferFrames, &callme, &g_data, &options );
    }
    catch( RtError& e )
    {
        cout << e.getMessage() << endl;
        exit( 1 );
    }
    
    try {
        /* start stream */
        audio.startStream();
        
        /* let GLUT handle the current thread from here */
        glutMainLoop();
        
        /* stop the stream. */
        audio.stopStream();
    }
    catch( RtError& e )
    {
        cout << e.getMessage() << endl;
        goto cleanup;
    }

    cleanup: 

    return 0;
}


void keyboardFunc( unsigned char key, int x, int y )
{
    switch(key) {
        case 'q': 
            auria_destroy(&g_data);
            exit(0);
            break;
        case 32: /* space */
            //g_data.pause = (g_data.pause == 0) ? 1 : 0;
            //g_data.mode = (g_data.mode == AURIA_FREEZE) ? 
            //    AURIA_SCROLL : AURIA_FREEZE;

            if(g_data.mode == AURIA_SCROLL || g_data.mode == AURIA_REPLAY) {
                g_data.mode = AURIA_PLEASE_FREEZE;
            } else if(g_data.mode == AURIA_FREEZE) {
                g_data.mode = AURIA_PLEASE_REPLAY;
                //g_data.wtpos = g_data.mincer->wtpos;
            }

            break;
        default:
            break;
    }
    glutPostRedisplay( );
}

void reshapeFunc( GLsizei w, GLsizei h )
{
    /* save the new window size */
    g_width = w; g_height = h;
    g_data.w = w;
    g_data.h = h;
    /* map the view port to the client area */
    glViewport( 0, 0, w, h );
    /* set the matrix mode to project */
    glMatrixMode( GL_PROJECTION );
    /* load the identity matrix */
    glLoadIdentity( );
    /* create the viewing frustum */
    gluPerspective( 45.0, (GLfloat) w / (GLfloat) h, 1.0, 45.0 );
    /* set the matrix mode to modelview */
    glMatrixMode( GL_MODELVIEW );
    /* load the identity matrix */
    glLoadIdentity( );
    /* position the view point */
    gluLookAt( 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}

void initGfx()
{
    /* double buffer, use rgb color, enable depth buffer */
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    /* initialize the window size */
    g_data.w = g_width;
    g_data.h = g_height;
    glutInitWindowSize( g_width, g_height );
    /* set the window postion */
    glutInitWindowPosition( 100, 100 );
    /* create the window */
    glutCreateWindow( "simple" );
   
    /* set the idle function - called when idle */
    glutIdleFunc( idleFunc );
    /* set the display function - called when redrawing */
    glutDisplayFunc( displayFunc );
    /* set the reshape function - called when client area changes */
    glutReshapeFunc( reshapeFunc );
    /* set the keyboard function - called on keyboard events */
    glutKeyboardFunc( keyboardFunc );
    /* set the mouse function - called on mouse stuff */
    glutMouseFunc( mouseFunc );
    glutPassiveMotionFunc(passiveMotionFunc); 

    /* set clear color */
    glClearColor( 0, 0, 0, 1 );
    /* enable color material */
    glEnable( GL_COLOR_MATERIAL );
    /* enable depth test */
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
}


void mouseFunc( int button, int state, int x, int y )
{
    GLfloat depth = 0;
    GLdouble fX, fY, fZ;
    GLdouble  model[16], proj[16];
    GLint view[4];
    float rad, theta;
    if( button == GLUT_LEFT_BUTTON ) {
        /* when left mouse button is down */
        if( state == GLUT_DOWN ) {
            g_data.state = (g_data.state == 1) ? 0 : 1 ;
        } else {
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        /* when right mouse button down */
        if( state == GLUT_DOWN ) {
            g_data.state_Y = (g_data.state_Y == 1) ? 0 : 1 ;
        }
        else {
        }
    }
    else {
    }

    glutPostRedisplay( );
}
void passiveMotionFunc(int x, int y)
{
    if(g_data.mode == AURIA_FREEZE) {
        if(g_data.state == 1) {
            g_data.posX = 1.0 * x / g_data.w;
        }
        
        if(g_data.state_Y == 1) {
            g_data.posY = 1.0 * y / g_data.h;
        }
    }
}

int auria_init(auria_data *gd, char *filename)
{
    gd->sr = MY_SRATE;
    gd->state = 1;
    gd->posY = 0.5;
    gd->posX = 0;
    gd->level = 0;
    gd->offset = 0;
    gd->nbars = 500;
    gd->mode = AURIA_SCROLL;
    gd->soundbars = (float *)malloc(sizeof(float) * gd->nbars);
    auria_init_audio(gd, filename);
    gd->counter = 0;
    gd->counter_speed = (unsigned int) gd->wav->size / gd->nbars;
    gd->mincer_offset = 0;
    gd->wtpos = 0;
    gd->last = 0;

    unsigned int n;
    unsigned int skip = gd->wav->size / gd->nbars;
    SPFLOAT out = 0;

    //for(n = 0; n < gd->wav->size; n++) {
    //    plumber_compute(&gd->pd, PLUMBER_COMPUTE);
    //    out = sporth_stack_pop_float(&gd->pd.sporth.stack);
    //    gd->wav->tbl[n] = out;
    //}

    for(n = 0; n < gd->nbars; n++) {
        gd->soundbars[n] = gd->wav->tbl[n * skip];
    }

    gd->soundbars[0] = 1;
    return 0;
}

void idleFunc( )
{
    /* render the scene */
    glutPostRedisplay( );
}

int auria_destroy(auria_data *gd) 
{
    stop_audio();
    auria_destroy_audio(gd);
    free(gd->soundbars);
    return 0;
}
