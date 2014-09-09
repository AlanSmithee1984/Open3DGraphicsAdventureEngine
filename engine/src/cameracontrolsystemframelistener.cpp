#include "cameracontrolsystemframelistener.h"

#include <OIS/OIS.h>

#include <OgreRenderWindow.h>
#include <OgreVector3.h>
#include <OgreCamera.h>
#include <OgreStringConverter.h>
#include <OgreWindowEventUtilities.h>

#include <CCSCameraControlSystem.h>
#include <CCSBasicCameraModes.h>
#include <CCSFreeCameraMode.h>

#include <QDebug>


CameraControlSystemFrameListener::CameraControlSystemFrameListener(Ogre::RenderWindow * window, Ogre::SceneManager* sceneManager,
                                                                   Ogre::Camera *camera, Ogre::SceneNode* target)
    : m_window(window),
      m_sceneManager(sceneManager),
      m_camera(camera),
      m_targetNode(target),
      m_pCameraCS(NULL),
      mTranslateVector(Ogre::Vector3::ZERO),
      mTimeUntilNextToggle(0),
      mSceneDetailIndex(0),
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


    //    showDebugOverlay(true);

    this->initCameraControlSystem();

    //Register as a Window listener
    Q_ASSERT(m_window);
    Ogre::WindowEventUtilities::addWindowEventListener(m_window, this);


    //Set initial mouse clipping size
    this->windowResized(m_window);

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Q_ASSERT(root);
    root->addFrameListener(this);

}

CameraControlSystemFrameListener::~CameraControlSystemFrameListener()
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(m_window, this);
    windowClosed(m_window);
}

bool CameraControlSystemFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
    //Need to capture/update each device
    m_keyboard->capture();
    m_mouse->capture();

    if( m_joy )
    {
        m_joy->capture();
    }

    if( m_keyboard->isKeyDown(OIS::KC_ESCAPE) || m_keyboard->isKeyDown(OIS::KC_Q) )
    {
        return false;
    }

    m_pCameraCS->update(evt.timeSinceLastFrame);

    //    mKeyBuffer -= e.timeSinceLastFrame;

    return true;
}

bool CameraControlSystemFrameListener::frameRenderingQueued(const Ogre::FrameEvent &evt)
{


    Q_ASSERT( m_keyboard->buffered() == false);
    if( this->processUnbufferedKeyInput(evt) == false )
    {
        return false;
    }

    Q_ASSERT( !m_mouse->buffered() );
    if( this->processUnbufferedMouseInput(evt) == false )
    {
        return false;
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

    const Ogre::Vector3 camPos(300, 10500, 100);

    // -------------------------------------------------------------------------------------
    // Register a "Free" camera mode. In this mode the camera is controlled by the user.
    // The camera orientation is fixed to a yaw axis.

    CCS::FreeCameraMode* freeCamMode = new CCS::FreeCameraMode(m_pCameraCS, camPos);
    m_pCameraCS->registerCameraMode("Free", freeCamMode);
    freeCamMode->setMoveFactor(30);

//    freeCamMode->setCameraPosition(camPos);

//    const Ogre::Quaternion orient(Ogre::Radian(Ogre::Degree(90)), Ogre::Vector3::UNIT_Z);
//    freeCamMode->setCameraOrientation(orient);

    m_pCameraCS->setCurrentCameraMode(freeCamMode);

}


bool CameraControlSystemFrameListener::processUnbufferedKeyInput(const Ogre::FrameEvent& evt)
{
    this->processCameraKeyInput();

    using namespace OIS;



    if( m_keyboard->isKeyDown(KC_ESCAPE) || m_keyboard->isKeyDown(KC_Q) )
        return false;

    if( m_keyboard->isKeyDown(KC_F)  )
    {
        this->printStats();
    }


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

//    static bool displayCameraDetails = false;
//    if(m_keyboard->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
//    {
//        displayCameraDetails = !displayCameraDetails;
//        mTimeUntilNextToggle = 0.5;
//        if (!displayCameraDetails)
//            mDebugText = "";
//    }

//    // Print camera details
//    if(displayCameraDetails)
//        mDebugText = "P: " + Ogre::StringConverter::toString(m_camera->getDerivedPosition()) +
//                " " + "O: " + Ogre::StringConverter::toString(m_camera->getDerivedOrientation());

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
        if(m_pCameraCS->getCameraModeName(m_pCameraCS->getCurrentCameraMode()) == "Free")
        {
            CCS::FreeCameraMode* freeCameraMode = static_cast<CCS::FreeCameraMode*>(m_pCameraCS->getCameraMode("Free"));
            Q_ASSERT(freeCameraMode);

//            qDebug() << ms.X.rel << ms.Y.rel;

            freeCameraMode->yaw(ms.X.rel);
            freeCameraMode->pitch(ms.Y.rel);
        }
    }

    return true;
}

void CameraControlSystemFrameListener::processCameraKeyInput()
{

    CCS::FreeCameraMode* freeCameraMode = (CCS::FreeCameraMode*)m_pCameraCS->getCameraMode("Free");

    if(m_keyboard->isKeyDown(OIS::KC_A))
    {
        //        mTranslateVector.x = -m_moveScale;	// Move camera left
        freeCameraMode->goLeft();
    }

    if(m_keyboard->isKeyDown(OIS::KC_D))
    {
        //        mTranslateVector.x = +m_moveScale;	// Move camera RIGHT
        freeCameraMode->goRight();
    }

    if(m_keyboard->isKeyDown(OIS::KC_UP) || m_keyboard->isKeyDown(OIS::KC_W) )
    {
        //        mTranslateVector.z = -m_moveScale;	// Move camera forward
        freeCameraMode->goForward();
    }

    if(m_keyboard->isKeyDown(OIS::KC_DOWN) || m_keyboard->isKeyDown(OIS::KC_S) )
    {
        //        mTranslateVector.z = m_moveScale;	// Move camera backward
        freeCameraMode->goBackward();
    }

    if(m_keyboard->isKeyDown(OIS::KC_PGUP))
    {
        //        mTranslateVector.y = +m_moveScale;	// Move camera up
        freeCameraMode->goUp();
    }

    if(m_keyboard->isKeyDown(OIS::KC_PGDOWN))
    {
        //        mTranslateVector.y = -m_moveScale;	// Move camera down
        freeCameraMode->goDown();
    }

    if(m_keyboard->isKeyDown(OIS::KC_RIGHT))
    {
        //        m_camera->yaw(-m_rotScale);
        freeCameraMode->yaw(-1);
    }

    if(m_keyboard->isKeyDown(OIS::KC_LEFT))
    {
        //        m_camera->yaw(+m_rotScale);
        freeCameraMode->yaw(1);
    }
}

void CameraControlSystemFrameListener::windowResized(Ogre::RenderWindow *rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    qDebug() << "resize" << width << height;

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

