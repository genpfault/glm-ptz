#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#undef GLM_FORCE_RADIANS
#endif

class Camera
{
public:
    glm::dvec2 PlaneUnproject( const glm::dvec2& win, const bool apply = true )
    {
        if( apply ) Apply();
        const glm::dvec3 world1 = glm::unProject( glm::dvec3( win, 0.0 ), mModel, mProj, mViewport );
        const glm::dvec3 world2 = glm::unProject( glm::dvec3( win, 1.0 ), mModel, mProj, mViewport );

        // u is a value such that:
        // 0 = world1.z + u * ( world2.z - world1.z )
        double u = -world1.z / ( world2.z - world1.z );
        return glm::dvec2( world1 + u * ( world2 - world1 ) );
    }

    glm::dvec3 Project( const glm::dvec3& obj, const bool apply = true )
    {
        if( apply ) Apply();
        return glm::project( obj, mModel, mProj, mViewport );
    }

    double UnitsPerPixel( const bool apply = true )
    {
        if( apply ) ApplyUnitsPerPixel();
        const glm::dvec3 p0 = Project( glm::dvec3( 0, 0, 0 ), false );
        const glm::dvec3 p1 = Project( glm::dvec3( 1, 0, 0 ), false );
        return std::abs( p1.x - p0.x );
    }

    void Upload( const bool apply = true )
    {
        if( apply ) Apply();
        glViewport( mViewport[0], mViewport[1], mViewport[2], mViewport[3] );
        glMatrixMode( GL_PROJECTION );
        glLoadMatrixd( &mProj[0][0] );
        glMatrixMode( GL_MODELVIEW );
        glLoadMatrixd( &mModel[0][0] );
    }

    void UnUpload()
    {
        glViewport( mViewport[0], mViewport[1], mViewport[2], mViewport[3] );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, mViewport[2], 0, mViewport[3], -1, 1 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
    }

    glm::ivec4 mViewport;

protected:
    // convert derived state into modelview/projection matrices
    virtual void Apply() = 0;
    virtual void ApplyUnitsPerPixel() = 0;

    glm::dmat4 mModel, mProj;
};

class TiltZoomCamera : public Camera
{
public:
    TiltZoomCamera() : mCenter( 0, 0 ) , mRot( 0 ) , mPitch( -90 ), mZoom( 5 ) { }

    void Reset()
    {
        mPitch = -90;
        mRot = 0;
    }

    glm::dvec2 mCenter;
    double mRot, mPitch;    // degrees
    double mZoom;

private:
    virtual void Apply()
    {
        mPitch = glm::clamp( mPitch, -90.0, 90.0 );

        mProj = glm::perspective( glm::radians( 60.0 ), mViewport[2] / (double)mViewport[3], 0.1, 1000.0 );
        mModel = glm::dmat4( 1.0 );
        mModel = glm::translate( mModel, glm::dvec3( 0, 0, -mZoom ) );
        mModel = glm::rotate( mModel, glm::radians( -mPitch - 90 ), glm::dvec3( 1, 0, 0 ) );
        mModel = glm::rotate( mModel, glm::radians( mRot ), glm::dvec3( 0, 0, 1 ) );
        mModel = glm::translate( mModel, glm::dvec3( -mCenter.x, -mCenter.y, 0 ) );
    }

    virtual void ApplyUnitsPerPixel()
    {
        mProj = glm::perspective( glm::radians( 60.0 ), mViewport[2] / (double)mViewport[3], 0.1, 1000.0 );
        mModel = glm::dmat4( 1.0 );
        mModel = glm::translate( mModel, glm::dvec3( 0, 0, -mZoom ) );
    }
};

#endif