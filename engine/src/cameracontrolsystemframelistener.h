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

class CameraControlSystemFrameListener : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
    CameraControlSystemFrameListener(Ogre::RenderWindow * window, Ogre::Camera* camera);
    virtual ~CameraControlSystemFrameListener();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
    bool processUnbufferedKeyInput(const Ogre::FrameEvent &evt);
    bool processUnbufferedMouseInput(const Ogre::FrameEvent &evt);

    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);

    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);


    void moveCamera();

    Ogre::RenderWindow* m_window;
    Ogre::Camera* mCamera;

    //OIS Input devices
    OIS::InputManager* m_inputManager;
    OIS::Mouse*    m_mouse;
    OIS::Keyboard* m_keyboard;
    OIS::JoyStick* m_joy;

    float m_moveScale;
    float m_speedLimit;

    Ogre::Vector3 mTranslateVector;
    Ogre::Real mCurrentSpeed;

    Ogre::Real mMoveSpeed;
    Ogre::Degree mRotateSpeed;

    Ogre::Degree mRotScale;
    // just to stop toggles flipping too fast
    Ogre::Real mTimeUntilNextToggle ;
    Ogre::Radian mRotX, mRotY;


    int mSceneDetailIndex ;
    std::string mDebugText;
};

#endif // CAMERACONTROLSYSTEMFRAMELISTENER_H
