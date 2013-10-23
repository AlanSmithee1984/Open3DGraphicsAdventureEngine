#include "cameracontrolsystemframelistener.h"

#include <OIS/OIS.h>

#include <OgreRenderWindow.h>
#include <OgreVector3.h>
#include <OgreCamera.h>
#include <OgreStringConverter.h>


CameraControlSystemFrameListener::CameraControlSystemFrameListener(Ogre::RenderWindow * window)
    : m_window(window)
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    m_inputManager = OIS::InputManager::createInputSystem( pl );
}


bool CameraControlSystemFrameListener::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if(m_window->isClosed())	return false;

    mSpeedLimit = mMoveScale * evt.timeSinceLastFrame;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    if( m_joy ) m_joy->capture();

    bool buffJ = (m_joy) ? m_joy->buffered() : true;

    Ogre::Vector3 lastMotion = mTranslateVector;

    //Check if one of the devices is not buffered
    if( !mMouse->buffered() || !mKeyboard->buffered() || !buffJ )
    {
        // one of the input modes is immediate, so setup what is needed for immediate movement
        if (mTimeUntilNextToggle >= 0)
            mTimeUntilNextToggle -= evt.timeSinceLastFrame;

        // Move about 100 units per second
        mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
        // Take about 10 seconds for full rotation
        mRotScale = mRotateSpeed * evt.timeSinceLastFrame;

        mRotX = 0;
        mRotY = 0;
        mTranslateVector = Ogre::Vector3::ZERO;

    }

    //Check to see which device is not buffered, and handle it
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
    if( !mKeyboard->buffered() )
        if( processUnbufferedKeyInput(evt) == false )
            return false;

//#ifdef USE_RTSHADER_SYSTEM
//    processShaderGeneratorInput();
//#endif

#endif
    if( !mMouse->buffered() )
        if( processUnbufferedMouseInput(evt) == false )
            return false;

    // ramp up / ramp down speed
    if (mTranslateVector == Ogre::Vector3::ZERO)
    {
        // decay (one third speed)
        mCurrentSpeed -= evt.timeSinceLastFrame * 0.3;
        mTranslateVector = lastMotion;
    }
    else
    {
        // ramp up
        mCurrentSpeed += evt.timeSinceLastFrame;

    }
    // Limit motion speed
    if (mCurrentSpeed > 1.0)
        mCurrentSpeed = 1.0;
    if (mCurrentSpeed < 0.0)
        mCurrentSpeed = 0.0;

    mTranslateVector *= mCurrentSpeed;


    if( !mMouse->buffered() || !mKeyboard->buffered() || !buffJ )
        moveCamera();

    return true;
}


bool CameraControlSystemFrameListener::processUnbufferedKeyInput(const Ogre::FrameEvent& evt)
{
    using namespace OIS;

    if(mKeyboard->isKeyDown(KC_A))
        mTranslateVector.x = -mMoveScale;	// Move camera left

    if(mKeyboard->isKeyDown(KC_D))
        mTranslateVector.x = mMoveScale;	// Move camera RIGHT

    if(mKeyboard->isKeyDown(KC_UP) || mKeyboard->isKeyDown(KC_W) )
        mTranslateVector.z = -mMoveScale;	// Move camera forward

    if(mKeyboard->isKeyDown(KC_DOWN) || mKeyboard->isKeyDown(KC_S) )
        mTranslateVector.z = mMoveScale;	// Move camera backward

    if(mKeyboard->isKeyDown(KC_PGUP))
        mTranslateVector.y = mMoveScale;	// Move camera up

    if(mKeyboard->isKeyDown(KC_PGDOWN))
        mTranslateVector.y = -mMoveScale;	// Move camera down

    if(mKeyboard->isKeyDown(KC_RIGHT))
        mCamera->yaw(-mRotScale);

    if(mKeyboard->isKeyDown(KC_LEFT))
        mCamera->yaw(mRotScale);

    if( mKeyboard->isKeyDown(KC_ESCAPE) || mKeyboard->isKeyDown(KC_Q) )
        return false;

//    if( mKeyboard->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0 )
//    {
//        mStatsOn = !mStatsOn;
//        showDebugOverlay(mStatsOn);
//        mTimeUntilNextToggle = 1;
//    }

//    if( mKeyboard->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0 )
//    {
//        switch(mFiltering)
//        {
//        case TFO_BILINEAR:
//            mFiltering = TFO_TRILINEAR;
//            mAniso = 1;
//            break;
//        case TFO_TRILINEAR:
//            mFiltering = TFO_ANISOTROPIC;
//            mAniso = 8;
//            break;
//        case TFO_ANISOTROPIC:
//            mFiltering = TFO_BILINEAR;
//            mAniso = 1;
//            break;
//        default: break;
//        }
//        MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
//        MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);

//        showDebugOverlay(mStatsOn);
//        mTimeUntilNextToggle = 1;
//    }

//    if(mKeyboard->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
//    {
//        std::ostringstream ss;
//        ss << "screenshot_" << ++mNumScreenShots << ".png";
//        mWindow->writeContentsToFile(ss.str());
//        mTimeUntilNextToggle = 0.5;
//        mDebugText = "Saved: " + ss.str();
//    }

    if(mKeyboard->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
    {
        mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
        switch(mSceneDetailIndex) {
            case 0 : mCamera->setPolygonMode(Ogre::PM_SOLID); break;
            case 1 : mCamera->setPolygonMode(Ogre::PM_WIREFRAME); break;
            case 2 : mCamera->setPolygonMode(Ogre::PM_POINTS); break;
        }
        mTimeUntilNextToggle = 0.5;
    }

    static bool displayCameraDetails = false;
    if(mKeyboard->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
    {
        displayCameraDetails = !displayCameraDetails;
        mTimeUntilNextToggle = 0.5;
        if (!displayCameraDetails)
            mDebugText = "";
    }

    // Print camera details
    if(displayCameraDetails)
        mDebugText = "P: " + Ogre::StringConverter::toString(mCamera->getDerivedPosition()) +
                     " " + "O: " + Ogre::StringConverter::toString(mCamera->getDerivedOrientation());

    // Return true to continue rendering
    return true;
}


bool CameraControlSystemFrameListener::processUnbufferedMouseInput(const Ogre::FrameEvent& evt)
{

    // Rotation factors, may not be used if the second mouse button is pressed
    // 2nd mouse button - slide, otherwise rotate
    const OIS::MouseState &ms = mMouse->getMouseState();
    if( ms.buttonDown( OIS::MB_Right ) )
    {
        mTranslateVector.x += ms.X.rel * 0.13;
        mTranslateVector.y -= ms.Y.rel * 0.13;
    }
    else
    {
        mRotX = Ogre::Degree(-ms.X.rel * 0.13);
        mRotY = Ogre::Degree(-ms.Y.rel * 0.13);
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
        // Adjust the input depending upon viewport orientation
        Radian origRotY, origRotX;
        switch(mCamera->getViewport()->getOrientation())
        {
            case Viewport::OR_LANDSCAPELEFT:
                origRotY = mRotY;
                origRotX = mRotX;
                mRotX = origRotY;
                mRotY = -origRotX;
                break;
            case Viewport::OR_LANDSCAPERIGHT:
                origRotY = mRotY;
                origRotX = mRotX;
                mRotX = -origRotY;
                mRotY = origRotX;
                break;

            // Portrait doesn't need any change
            case Viewport::OR_PORTRAIT:
            default:
                break;
        }
#endif
    }

    return true;
}

void CameraControlSystemFrameListener::moveCamera()
{
    // Make all the changes to the camera
    // Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW
    //(e.g. airplane)
    mCamera->yaw(mRotX);
    mCamera->pitch(mRotY);
    mCamera->moveRelative(mTranslateVector);
}
