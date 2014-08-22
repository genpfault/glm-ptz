#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Camera.h"
#include <vector>

class MouseControl
{
public:
    enum Button { NONE, LEFT, MIDDLE, RIGHT };
};

class TiltZoomControl : public MouseControl
{
public:
    TiltZoomControl( TiltZoomCamera& camera ) : mCamera( camera ) { }

    void Mouse( const Button button, bool down, const glm::ivec2& pos )
    {
        mCurButton = button;
        if( MIDDLE == button && down )
        {
            mMouseStart = pos;
            mCameraStart = mCamera;
        }
        if( RIGHT == button && down )
        {
            mCenterStart = mCamera.PlaneUnproject( glm::dvec2( pos ) );
        }
        if( !down )
        {
            mCurButton = NONE;
        }
    }

    void Motion( const glm::ivec2& pos )
    {
        if( MIDDLE == mCurButton )
        {
            // make window width worth of mouse movement result in 180 degrees of rotation
            const double pitchFactor = 90.0 / mCamera.mViewport[2];
            // make window height worth of mouse movement result in 90 degrees of pitch
            const double rotFactor = 90.0 / mCamera.mViewport[3];

            mCamera.mRot = mCameraStart.mRot - ( pos.x - mMouseStart.x ) * rotFactor;
            mCamera.mPitch = mCameraStart.mPitch - ( pos.y - mMouseStart.y ) * pitchFactor;
        }

        if( RIGHT == mCurButton )
        {
            const glm::dvec2 cur = mCamera.PlaneUnproject( glm::dvec2( pos ) );
            mCamera.mCenter += ( mCenterStart - cur );
        }
    }

    void Wheel( int direction, const glm::ivec2& pos )
    {
        const glm::dvec2 beforeZoom = mCamera.PlaneUnproject( glm::dvec2( pos ) );
        const double scaleFactor = 0.90;
        if( direction == -1 )   mCamera.mZoom /= scaleFactor;
        if( direction ==  1 )   mCamera.mZoom *= scaleFactor;
        const glm::dvec2 afterZoom = mCamera.PlaneUnproject( glm::dvec2( pos ) );
        mCamera.mCenter += ( beforeZoom - afterZoom );
    }

private:
    TiltZoomCamera& mCamera;
    TiltZoomCamera mCameraStart;

    Button mCurButton;
    glm::ivec2 mMouseStart;
    glm::dvec2 mCenterStart;
    double mRotStart, mPitchStart;
};


class PointControl : public MouseControl
{
public:
    PointControl( Camera& camera ) : mCamera( camera ), mCurPoint( -1 ) { }

    void Mouse( const Button button, bool down, const glm::ivec2& pos )
    {
        mCurButton = button;
        if( !down || button != LEFT )
            mCurPoint = -1;

        mCurPoint = -1;
        glm::dvec3 target( pos, 0.0 );
        for( size_t i = 0; i < mPoints.size(); ++i )
        {
            glm::dvec3 pt = mCamera.Project( glm::dvec3( mPoints[i], 0.0 ) );
            double len = glm::length( pt - target );
            if( len < 10.0 )
            {
                mCurPoint = static_cast< int >( i );
                break;
            }
        }
    }

    void Motion( const glm::ivec2& pos )
    {
        if( mCurPoint < 0 || mCurButton != LEFT )
            return;

        glm::dvec2 target = mCamera.PlaneUnproject( glm::dvec2( pos ) );
        mPoints[ mCurPoint ] = glm::vec2( target );
        
        // snap-to-grid
        //mPoints[ mCurPoint ] = glm::round( mPoints[ mCurPoint ] );
    }

    std::vector< glm::vec2 > mPoints;

private:
    int mCurPoint;
    Button mCurButton;
    Camera& mCamera;
};

#endif