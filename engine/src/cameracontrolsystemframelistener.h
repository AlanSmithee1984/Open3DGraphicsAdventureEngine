#ifndef CAMERACONTROLSYSTEMFRAMELISTENER_H
#define CAMERACONTROLSYSTEMFRAMELISTENER_H

#include <OgreFrameListener.h>
#include <OgreVector3.h>

namespace OIS
{
class InputManager;
class Mouse;
class Keyboard;
class JoyStick;
}

class CameraControlSystemFrameListener: public Ogre::FrameListener
{
public:
    CameraControlSystemFrameListener(Ogre::RenderWindow * window);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
    bool processUnbufferedKeyInput(const Ogre::FrameEvent &evt);
    bool processUnbufferedMouseInput(const Ogre::FrameEvent &evt);

    void moveCamera();

    Ogre::RenderWindow* m_window;
    Ogre::Camera* mCamera;

    //OIS Input devices
    OIS::InputManager* m_inputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
    OIS::JoyStick* m_joy;

    float mMoveScale;
    float mSpeedLimit;

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
