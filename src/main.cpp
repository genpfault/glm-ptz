#include "Controller.h"
#include "Camera.h"
#include "GlUtil.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>

using namespace glm;


TiltZoomCamera camera;
TiltZoomControl tzControl( camera );
PointControl ptControl( camera );


void mouse( int button, int state, int x, int y )
{
    const ivec2 pos( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
    MouseControl::Button btn;
    if( GLUT_LEFT_BUTTON   == button ) btn = MouseControl::LEFT;
    if( GLUT_MIDDLE_BUTTON == button ) btn = MouseControl::MIDDLE;
    if( GLUT_RIGHT_BUTTON  == button ) btn = MouseControl::RIGHT;
    bool down = ( state == GLUT_DOWN );

    tzControl.Mouse( btn, down, pos );
    ptControl.Mouse( btn, down, pos );
    glutPostRedisplay();
}

void motion( int x, int y )
{
    const ivec2 pos( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
    tzControl.Motion( pos );
    ptControl.Motion( pos );
    glutPostRedisplay();
}

void wheel( int wheel, int direction, int x, int y )
{
    const ivec2 pos( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
    tzControl.Wheel( direction, pos );
    glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y )
{
    if( 27 == key )     exit( 0 );
    if( 'r' == key )    camera.Reset();
    if( 'c' == key )    ptControl.mPoints.clear();
    if( 'a' == key )
    {
        const dvec2 mpos( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
        const dvec2 pos = camera.PlaneUnproject( mpos );
        ptControl.mPoints.push_back( vec2( pos ) );
    }

    glutPostRedisplay();
}

void reshape( int w, int h )
{
    camera.mViewport = ivec4( 0, 0, w, h );
}

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    camera.Upload();

    glDisable( GL_DEPTH_TEST );

    glLineWidth( 1 );
    GlUtil::glMultigrid( 200 );

    if( !ptControl.mPoints.empty() )
    {
        GlUtil::glFancyLine( &ptControl.mPoints[0], ptControl.mPoints.size() );
    }

    // center/tilt point
    glPointSize( 3 );
    glBegin( GL_POINTS );
    glColor3ub( 255, 255, 255 );
    glVertex2dv( &camera.mCenter[0] );
    glEnd();

    glEnable( GL_DEPTH_TEST );

    glutSwapBuffers();
}

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 648, 480 );
    glutCreateWindow( "PanTiltZoom" );

    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutMouseWheelFunc( wheel );
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    if( GLEW_OK != glewInit() )
        return 1;

    glutMainLoop();
    return 0;
}