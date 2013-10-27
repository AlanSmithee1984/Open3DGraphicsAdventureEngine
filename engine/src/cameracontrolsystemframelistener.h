#ifndef CAMERACONTROLSYSTEMFRAMELISTENER_H
#define CAMERACONTROLSYSTEMFRAMELISTENER_H

#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector3.h>

namespace OIS
{
class InputManager;
class Mouse;
class Keyboard;
class JoyStick;
}

namespace CCS
{
class CameraControlSystem;
}

class CameraControlSystemFrameListener : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
    CameraControlSystemFrameListener(Ogre::RenderWindow * window, Ogre::SceneManager *sceneManager, Ogre::Camera* camera, Ogre::SceneNode *target = NULL);
    virtual ~CameraControlSystemFrameListener();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
    void initCameraControlSystem();

    bool processUnbufferedKeyInput(const Ogre::FrameEvent &evt);
    bool processUnbufferedMouseInput(const Ogre::FrameEvent &evt);

    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);

    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);


    void moveCamera();

    Ogre::RenderWindow* m_window;
    Ogre::SceneManager* m_sceneManager;
    Ogre::Camera* m_camera;
    Ogre::SceneNode* m_targetNode;

    CCS::CameraControlSystem* m_pCameraCS;

    Ogre::Vector3 mTranslateVector;

    float m_moveScale;
    float m_speedLimit;


    Ogre::Real mCurrentSpeed;


    Ogre::Degree m_rotScale;
    // just to stop toggles flipping too fast
    Ogre::Real mTimeUntilNextToggle ;
    int mSceneDetailIndex ;

    Ogre::Radian mRotX, mRotY;

    std::string mDebugText;


    Ogre::Real mMoveSpeed;
    Ogre::Degree mRotateSpeed;

    //OIS Input devices
    OIS::InputManager* m_inputManager;
    OIS::Mouse*    m_mouse;
    OIS::Keyboard* m_keyboard;
    OIS::JoyStick* m_joy;
};

#endif // CAMERACONTROLSYSTEMFRAMELISTENER_H
