#include "cameracontrolsystemframelistener.h"

#include <OIS/OIS.h>

#include <OgreRenderWindow.h>
#include <OgreVector3.h>
#include <OgreCamera.h>
#include <OgreStringConverter.h>
#include <OgreWindowEventUtilities.h>


CameraControlSystemFrameListener::CameraControlSystemFrameListener(Ogre::RenderWindow * window, Ogre::Camera *camera)
    : m_window(window),
      mCamera(camera),
      mTranslateVector(Ogre::Vector3::ZERO),
      m_moveScale(0.0f),
      m_rotScale(0.0f),
      mTimeUntilNextToggle(0),
      mSceneDetailIndex(0),
      mMoveSpeed(100),
      mRotateSpeed(36),
      m_inputManager(0),
      m_mouse(0),
      m_keyboard(0),
      m_joy(0)
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    m_inputManager = OIS::InputManager::createInputSystem( pl );


    //Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)

    bool bufferedKeys = false;
    bool bufferedMouse = false;
    bool bufferedJoy = false;

    m_keyboard = static_cast<OIS::Keyboard*>(m_inputManager->createInputObject( OIS::OISKeyboard, bufferedKeys ));
    m_mouse = static_cast<OIS::Mouse*>(m_inputManager->createInputObject( OIS::OISMouse, bufferedMouse ));
    try {
        m_joy = static_cast<OIS::JoyStick*>(m_inputManager->createInputObject( OIS::OISJoyStick, bufferedJoy ));
    }
    catch(...) {
        m_joy = 0;
    }

    //Set initial mouse clipping size
    this->windowResized(m_window);

//    showDebugOverlay(true);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(m_window, this);
}

CameraControlSystemFrameListener::~CameraControlSystemFrameListener()
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(m_window, this);
    windowClosed(m_window);
}


bool CameraControlSystemFrameListener::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if(m_window->isClosed())
    {
        return false;
    }

    m_speedLimit = m_moveScale * evt.timeSinceLastFrame;

    //Need to capture/update each device
    m_keyboard->capture();
    m_mouse->capture();
    if( m_joy ) m_joy->capture();

    bool buffJ = (m_joy) ? m_joy->buffered() : true;

    Ogre::Vector3 lastMotion = mTranslateVector;

    //Check if one of the devices is not buffered
    if( !m_mouse->buffered() || !m_keyboard->buffered() || !buffJ )
    {
        // one of the input modes is immediate, so setup what is needed for immediate movement
        if (mTimeUntilNextToggle >= 0)
            mTimeUntilNextToggle -= evt.timeSinceLastFrame;

        // Move about 100 units per second
        m_moveScale = mMoveSpeed * evt.timeSinceLastFrame;
        // Take about 10 seconds for full rotation
        m_rotScale = mRotateSpeed * evt.timeSinceLastFrame;

        mRotX = 0;
        mRotY = 0;
        mTranslateVector = Ogre::Vector3::ZERO;

    }

    //Check to see which device is not buffered, and handle it
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
    if( !m_keyboard->buffered() )
        if( processUnbufferedKeyInput(evt) == false )
            return false;

//#ifdef USE_RTSHADER_SYSTEM
//    processShaderGeneratorInput();
//#endif

#endif
    if( !m_mouse->buffered() )
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


    if( !m_mouse->buffered() || !m_keyboard->buffered() || !buffJ )
        moveCamera();

    return true;
}


bool CameraControlSystemFrameListener::processUnbufferedKeyInput(const Ogre::FrameEvent& evt)
{
    using namespace OIS;

    if(m_keyboard->isKeyDown(KC_A))
        mTranslateVector.x = -m_moveScale;	// Move camera left

    if(m_keyboard->isKeyDown(KC_D))
        mTranslateVector.x = m_moveScale;	// Move camera RIGHT

    if(m_keyboard->isKeyDown(KC_UP) || m_keyboard->isKeyDown(KC_W) )
        mTranslateVector.z = -m_moveScale;	// Move camera forward

    if(m_keyboard->isKeyDown(KC_DOWN) || m_keyboard->isKeyDown(KC_S) )
        mTranslateVector.z = m_moveScale;	// Move camera backward

    if(m_keyboard->isKeyDown(KC_PGUP))
        mTranslateVector.y = m_moveScale;	// Move camera up

    if(m_keyboard->isKeyDown(KC_PGDOWN))
        mTranslateVector.y = -m_moveScale;	// Move camera down

    if(m_keyboard->isKeyDown(KC_RIGHT))
        mCamera->yaw(-m_rotScale);

    if(m_keyboard->isKeyDown(KC_LEFT))
        mCamera->yaw(m_rotScale);

    if( m_keyboard->isKeyDown(KC_ESCAPE) || m_keyboard->isKeyDown(KC_Q) )
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

    if(m_keyboard->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
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
    if(m_keyboard->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
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
    const OIS::MouseState &ms = m_mouse->getMouseState();
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

void CameraControlSystemFrameListener::windowResized(Ogre::RenderWindow *rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = m_mouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void CameraControlSystemFrameListener::windowClosed(Ogre::RenderWindow *rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == m_window )
    {
        if( m_inputManager )
        {
            m_inputManager->destroyInputObject( m_mouse );
            m_inputManager->destroyInputObject( m_keyboard );
            m_inputManager->destroyInputObject( m_joy );

            OIS::InputManager::destroyInputSystem(m_inputManager);
            m_inputManager = 0;
        }
    }
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
