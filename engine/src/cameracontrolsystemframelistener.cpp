#include "cameracontrolsystemframelistener.h"

#include <OIS/OIS.h>

#include <OgreRenderWindow.h>
#include <OgreVector3.h>
#include <OgreCamera.h>
#include <OgreStringConverter.h>
#include <OgreWindowEventUtilities.h>

#include <CCSCameraControlSystem.h>
#include <CCSBasicCameraModes.h>

#include <QDebug>


CameraControlSystemFrameListener::CameraControlSystemFrameListener(Ogre::RenderWindow * window, Ogre::SceneManager* sceneManager, Ogre::Camera *camera, Ogre::SceneNode* target)
    : m_window(window),
      m_sceneManager(sceneManager),
      m_camera(camera),
      m_targetNode(target),
      m_pCameraCS(NULL),
      mTranslateVector(Ogre::Vector3::ZERO),
      m_moveScale(0.0f),
      m_rotScale(0.0f),
      mTimeUntilNextToggle(0),
      mSceneDetailIndex(0),
      mMoveSpeed(500),
      mRotateSpeed(20),
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
    try
    {
        m_joy = static_cast<OIS::JoyStick*>(m_inputManager->createInputObject( OIS::OISJoyStick, bufferedJoy ));
    }
    catch(...)
    {
        m_joy = 0;
    }

    //Set initial mouse clipping size
    this->windowResized(m_window);

//    showDebugOverlay(true);

    this->initCameraControlSystem();

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(m_window, this);

    Ogre::Root::getSingletonPtr()->addFrameListener(this);
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
    {
        if( processUnbufferedMouseInput(evt) == false )
        {
            return false;
        }
    }

    // ramp up / ramp down speed
    if (mTranslateVector == Ogre::Vector3::ZERO)
    {
        // decay (one third speed)
        mCurrentSpeed -= evt.timeSinceLastFrame * 0.8;
        mTranslateVector = lastMotion;
    }
    else
    {
        // ramp up
        mCurrentSpeed += evt.timeSinceLastFrame * 20;

    }

    // Limit motion speed
    if (mCurrentSpeed > 1.0)
        mCurrentSpeed = 1.0;
    if (mCurrentSpeed < 0.0)
        mCurrentSpeed = 0.0;

    mTranslateVector *= mCurrentSpeed;


    if( !m_mouse->buffered() || !m_keyboard->buffered() || !buffJ )
    {
        this->moveCamera();
    }

    return true;
}

Ogre::SceneNode *CameraControlSystemFrameListener::getCameraSceneNode() const
{
    return m_pCameraCS->getCameraSceneNode();
}


void CameraControlSystemFrameListener::initCameraControlSystem()
{
    // Create the camera system using the previously created ogre camera.
    m_pCameraCS = new CCS::CameraControlSystem(m_sceneManager, "CameraControlSystem", m_camera);

    // -------------------------------------------------------------------------------------
    // Register a "Fixed" camera mode. In this mode the camera position and orientation
    // never change.

//    CCS::FixedCameraMode* camMode1;
//    camMode1 = new CCS::FixedCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("Fixed (2)",camMode1);
//    camMode1->setCameraPosition(Ogre::Vector3(-500,0,-500));
//    float roll = 0; float yaw = 225; float pitch = 10;
//    camMode1->setCameraOrientation( Ogre::Quaternion(Ogre::Radian(Ogre::Degree(roll)),Ogre::Vector3::UNIT_Z)
//        * Ogre::Quaternion(Radian(Degree(yaw)),Vector3::UNIT_Y)
//        * Ogre::Quaternion(Radian(Degree(pitch)),Vector3::UNIT_X));

    // -------------------------------------------------------------------------------------
    // Register another "Fixed" camera mode with a different camera position.

//    camMode1 = new CCS::FixedCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("Fixed",camMode1);
//    //camMode1->setCameraPosition(Ogre::Vector3(PLANE_SIZE,PLANE_SIZE,PLANE_SIZE));
//    camMode1->setCameraPosition(Ogre::Vector3(1500,500,1500));
//    roll = 0; yaw = 45; pitch = -20;
//    camMode1->setCameraOrientation( Quaternion(Radian(Degree(roll)),Vector3::UNIT_Z)
//        * Quaternion(Radian(Degree(yaw)),Vector3::UNIT_Y)
//        * Quaternion(Radian(Degree(pitch)),Vector3::UNIT_X));

    // -------------------------------------------------------------------------------------
    // Register a "FixedTracking" camera mode. In this mode the camera position is fixed
    // and the camera always points to the target.

//    CCS::FixedTrackingCameraMode* camMode2;
//    camMode2 = new CCS::FixedTrackingCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("FixedTracking",camMode2);
//    camMode2->setCameraPosition(Ogre::Vector3(500,0,-100));

    // -------------------------------------------------------------------------------------
    // Register a "Chase" camera mode with default tightness (0.01). In
    // this mode the camera follows the target. The second parameter is the relative position
    // to the target. The orientation of the camera is fixed by a yaw axis (UNIT_Y by default).

    CCS::ChaseCameraMode* camMode3= new CCS::ChaseCameraMode(m_pCameraCS, Ogre::Vector3(0, 200, 200));
    m_pCameraCS->registerCameraMode("Chase(0.01 tightness)", camMode3);

    // -------------------------------------------------------------------------------------
    // Register another "ChaseCameraMode" camera mode with max tightness value.

//    CCS::FirstPersonCameraMode* camMode3 = new CCS::ChaseCameraMode(m_pCameraCS, Ogre::Vector3(0,0,-200));
//    m_pCameraCS->registerCameraMode("Chase(0.2 tightness)",camMode3);
//    camMode3->setCameraTightness(0.2);


    // -------------------------------------------------------------------------------------
    // Register a "ChaseFreeYawAxis" camera mode with max tightness. This mode is
    // similar to "Chase" camera mode but the camera orientation is not fixed by
    // a yaw axis. The camera orientation will be the same as the target.

//    camMode3 = new CCS::ChaseFreeYawAxisCameraMode(m_pCameraCS,Ogre::Vector3(0,0,-200)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("ChaseFreeYawAxis(0.01 tightness)",camMode3);
//    camMode3->setCameraTightness(0.05);

    // -------------------------------------------------------------------------------------
    // Register another "ChaseFreeYawAxis" camera mode with 0.05 tightness value.

//    camMode3 = new CCS::ChaseFreeYawAxisCameraMode(m_pCameraCS,Ogre::Vector3(0,0,-200)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("ChaseFreeYawAxis(0.1 tightness)",camMode3);
//    camMode3->setCameraTightness(0.3);


    // -------------------------------------------------------------------------------------
    // Register a "FirstPerson" camera mode.

    CCS::FirstPersonCameraMode* camMode4 = new CCS::FirstPersonCameraMode(m_pCameraCS,
                                                                          Ogre::Vector3(2000, 500, 1000),
                                                                          Ogre::Radian(Ogre::Degree(0)),
                                                                          Ogre::Radian(Ogre::Degree(45)),
                                                                          Ogre::Radian(Ogre::Degree(0)));
    m_pCameraCS->registerCameraMode("FirstPerson", camMode4);

    // -------------------------------------------------------------------------------------
    // Register another "FirstPerson" camera mode where the character is hidden.

//    camMode4 = new CCS::FirstPersonCameraMode(m_pCameraCS,Ogre::Vector3(0,17,-16)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("FirstPerson (character hidden)",camMode4);
//    camMode4->setCharacterVisible(false);

    // -------------------------------------------------------------------------------------
    // Register a "PlaneBinded" camera mode. In this mode the camera is constrained to the
    // limits of a plane. The camera always points to the target, perpendicularly to the plane.

//    Ogre::Plane* mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Z, Ogre::Vector3(0,0,-200));
//    CCS::PlaneBindedCameraMode* camMode5 = new CCS::PlaneBindedCameraMode(m_pCameraCS, *mPlane);
//    m_pCameraCS->registerCameraMode("PlaneBinded (XY)",camMode5);

//    // -------------------------------------------------------------------------------------
//    // Register another "PlaneBinded" camera mode using a top point of view.

//    mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0,1000,0));
//    camMode5 = new CCS::PlaneBindedCameraMode(m_pCameraCS, *mPlane, Ogre::Vector3::UNIT_Z);
//    m_pCameraCS->registerCameraMode("PlaneBinded (XZ)",camMode5);

//    // -------------------------------------------------------------------------------------
//    // Register a "ThroughTarget" camera mode. In this mode the camera points to a given
//    // position (the "focus") throuh the target. The camera orientation is fixed by a yaw axis.

//    CCS::ThroughTargetCameraMode* camMode6 = new CCS::ThroughTargetCameraMode(m_pCameraCS, 400);
//    m_pCameraCS->registerCameraMode("ThroughTarget",camMode6);
//    camMode6->setCameraFocusPosition(atheneNode->_getDerivedPosition() - Ogre::Vector3(0,100,0));

//    // -------------------------------------------------------------------------------------
//    // Register another "ThroughTarget" camera mode. In this case the "focus" is the
//    // target and vice versa.

//    camMode6 = new CCS::ThroughTargetCameraMode(m_pCameraCS, 400, true);
//    m_pCameraCS->registerCameraMode("ThroughTarget (inverse)",camMode6);
//    camMode6->setCameraFocusPosition(atheneNode->_getDerivedPosition() + Ogre::Vector3(0,100,0));

//    // -------------------------------------------------------------------------------------
//    // Register a "ClosestToTarget" camera mode. In this camera mode the position of the
//    // camera is chosen to be the closest to the target of a given list. The camera
//    // orientation is fixed by a yaw axis.

//    CCS::ClosestToTargetCameraMode* camMode7 = new CCS::ClosestToTargetCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("ClosestToTarget",camMode7);

//    Ogre::Vector3* camPos1 = new Ogre::Vector3(-400,0,-400);
//    Ogre::Vector3* camPos2 = new Ogre::Vector3(-400,0,1400);
//    Ogre::Vector3* camPos3 = new Ogre::Vector3(1400,0,1400);

//    camMode7->addCameraPosition(*camPos1);
//    camMode7->addCameraPosition(*camPos2);
//    camMode7->addCameraPosition(*camPos3);

//    // -------------------------------------------------------------------------------------
//    // Register an "Attached" camera mode. In this mode the camera node is attached to the
//    // target node as a child.

//    CCS::AttachedCameraMode* camMode8 = new CCS::AttachedCameraMode(m_pCameraCS,Ogre::Vector3(200,0,0)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(90)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("Attached (lateral)",camMode8);

//    camMode8 = new CCS::AttachedCameraMode(m_pCameraCS,Ogre::Vector3(0,50,-200)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("Attached (back/top)",camMode8);

//    // -------------------------------------------------------------------------------------
//    // Register a "Free" camera mode. In this mode the camera is controlled by the user.
//    // The camera orientation is fixed to a yaw axis.

//    CCS::FreeCameraMode* camMode9 = new CCS::FreeCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("Free",camMode9);
//    camMode9->setMoveFactor(30);

//    // -------------------------------------------------------------------------------------
//    // Register a "FixedDirection" camera mode. In this mode the
//    // target is always seen from the same point of view.

//    CCS::FixedDirectionCameraMode* camMode10 = new CCS::FixedDirectionCameraMode(m_pCameraCS, Ogre::Vector3(-1,-1,-1), 1000);
//    m_pCameraCS->registerCameraMode("Fixed direction",camMode10);

//    // -------------------------------------------------------------------------------------
//    // Register another "FixedDirection" camera mode with a 0.01 tightness value.

//    camMode10 = new CCS::FixedDirectionCameraMode(m_pCameraCS, Ogre::Vector3(-1,-1,-1), 1000);
//    m_pCameraCS->registerCameraMode("Fixed direction (0.01 tightness)",camMode10);
//    camMode10->setCameraTightness(0.01);

//    // -------------------------------------------------------------------------------------
//    // Register an "Orbital" camera mode. This is basically an attached camera mode where the user
//    // can mofify the camera position. If the scene focus is seen as the center of a sphere, the camera rotates arount it

//    CCS::OrbitalCameraMode* camMode12 = new CCS::OrbitalCameraMode(m_pCameraCS, 200);
//    m_pCameraCS->registerCameraMode("Orbital",camMode12);
//    camMode12->setZoomFactor(100);
//    camMode12->setRotationFactor(50);

//    // -------------------------------------------------------------------------------------
//    // Register a collidable orbital camera mode.
//    //

//    CCS::OrbitalCameraMode* camMode13 = new CCS::OrbitalCameraMode(m_pCameraCS, 200);
//    m_pCameraCS->registerCameraMode("Orbital (collidable)",camMode13);
//    camMode13->setZoomFactor(100);
//    camMode13->setRotationFactor(50);
//    camMode13->setZoom(400);
//    camMode13->setYaw(Radian(Degree(90)));
//    camMode13->setCollisionsEnabled(true);
//    // ** Uncomment for custom collisions calculation. By default the collisions are based on ogre basic raycast feature **
//    //camMode13->collisionDelegate = CCS::CollidableCamera::newCollisionDelegate(this
//    //	, &CameraControlSystemDemo::CustomCollisionDetectionFunction);


//    // -------------------------------------------------------------------------------------
//    // Register the custom "Dummy" camera mode defined previously. It basically goes forward
//    // and backward constantly

//    DummyCameraMode* camMode14 = new DummyCameraMode(m_pCameraCS, 400);
//    m_pCameraCS->registerCameraMode("Dummy",(CCS::CameraControlSystem::CameraMode*)camMode14);

    // Set the camera target
    if(m_targetNode == NULL)
    {
        m_targetNode = m_sceneManager->createSceneNode();
    }
    m_pCameraCS->setCameraTarget(m_targetNode);

    m_pCameraCS->setCurrentCameraMode(camMode4);



}


bool CameraControlSystemFrameListener::processUnbufferedKeyInput(const Ogre::FrameEvent& evt)
{
    using namespace OIS;

    if(m_keyboard->isKeyDown(KC_A))
    {
       mTranslateVector.x = -m_moveScale;	// Move camera left
    }

    if(m_keyboard->isKeyDown(KC_D))
    {
        mTranslateVector.x = +m_moveScale;	// Move camera RIGHT
    }

    if(m_keyboard->isKeyDown(KC_UP) || m_keyboard->isKeyDown(KC_W) )
        mTranslateVector.z = -m_moveScale;	// Move camera forward

    if(m_keyboard->isKeyDown(KC_DOWN) || m_keyboard->isKeyDown(KC_S) )
        mTranslateVector.z = m_moveScale;	// Move camera backward

    if(m_keyboard->isKeyDown(KC_PGUP))
    {
        mTranslateVector.y = +m_moveScale;	// Move camera up
    }

    if(m_keyboard->isKeyDown(KC_PGDOWN))
    {
        mTranslateVector.y = -m_moveScale;	// Move camera down
    }

    if(m_keyboard->isKeyDown(KC_RIGHT))
    {
        m_camera->yaw(-m_rotScale);
    }

    if(m_keyboard->isKeyDown(KC_LEFT))
    {
        m_camera->yaw(+m_rotScale);
    }

    if( m_keyboard->isKeyDown(KC_ESCAPE) || m_keyboard->isKeyDown(KC_Q) )
        return false;

    if( m_keyboard->isKeyDown(KC_F)  )
    {
        this->printStats();
    }

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
            case 0 : m_camera->setPolygonMode(Ogre::PM_SOLID); break;
            case 1 : m_camera->setPolygonMode(Ogre::PM_WIREFRAME); break;
            case 2 : m_camera->setPolygonMode(Ogre::PM_POINTS); break;
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
        mDebugText = "P: " + Ogre::StringConverter::toString(m_camera->getDerivedPosition()) +
                     " " + "O: " + Ogre::StringConverter::toString(m_camera->getDerivedOrientation());

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
    m_camera->yaw(mRotX);
    m_camera->pitch(mRotY);

    m_camera->moveRelative(mTranslateVector);

}

void CameraControlSystemFrameListener::printStats()
{
    qDebug() << "Status Infos:";

    Ogre::RenderSystem::RenderTargetIterator iter =
            Ogre::Root::getSingletonPtr()->getRenderSystem()->getRenderTargetIterator();
    while (iter.hasMoreElements())
    {
        Ogre::RenderTarget* rt = iter.getNext();
        qDebug() << rt;
        qDebug() << "fps:" << rt->getAverageFPS();
        qDebug() << "batch count:" << rt->getBatchCount();
        qDebug() << "triangle count:" << rt->getTriangleCount();

    }

    Ogre::Vector3 pos = m_camera->getPosition();

    qDebug() << "Cam Position: " <<  pos.x << pos.y << pos.z;


    qDebug(); // new line


}

