#ifndef GLUTIL_H
#define GLUTIL_H

#include <GL/glew.h>

struct GlUtil
{
    static void glGrid( int size, int step )
    {
        for( int i = -size; i <= size; i += step )
        {
            glVertex2i( i, -size );
            glVertex2i( i,  size );
            glVertex2i( -size, i );
            glVertex2i(  size, i );
        }
    }

    static void glMultigrid( int size )
    {
        //glEnable( GL_LINE_SMOOTH );
        //glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin( GL_LINES );
        glColor3ub( 0, 32, 0 );
        glGrid( size, 1 );
        glColor3ub( 0, 64, 0 );
        glGrid( size, 5 );
        glColor3ub( 0, 128, 0 );
        glGrid( size, 10 );
        glEnd();

        //glDisable( GL_LINE_SMOOTH );
        //glDisable( GL_BLEND );
    }

    static void glLine( const glm::vec2* points, const size_t count )
    {
        for( size_t i = 0; i < count; ++i )
        {
            glVertex2fv( &points[i][0] );
        }
    }

    static void glLine( const glm::vec2& p0, const glm::vec2& p1 )
    {
        glBegin( GL_LINES );
        glVertex2fv( &p0[0] );
        glVertex2fv( &p1[0] );
        glEnd();
    }

    static void glFancyLine( const glm::vec2* points, const size_t count )
    {
        glLineWidth( 1 );
        glBegin( GL_LINE_STRIP );
        glColor3ub( 255, 255, 255 );
        glLine( points, count );
        glEnd();

        glPointSize( 7 );
        glBegin( GL_POINTS );
        glColor3ub( 255, 0, 0 );
        glLine( points, count );
        glEnd();
    }
};

#endif