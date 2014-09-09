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



    Ogre::SceneNode *getCameraSceneNode() const;

protected:
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

private:
    void initCameraControlSystem();

    bool processUnbufferedKeyInput(const Ogre::FrameEvent &evt);
    bool processUnbufferedMouseInput(const Ogre::FrameEvent &evt);

    void processCameraKeyInput();

    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);

    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);


    void printStats();

    Ogre::RenderWindow* m_window;
    Ogre::SceneManager* m_sceneManager;
    Ogre::Camera* m_camera;
    Ogre::SceneNode* m_targetNode;

    CCS::CameraControlSystem* m_pCameraCS;

    Ogre::Vector3 mTranslateVector;


    // just to stop toggles flipping too fast
    Ogre::Real mTimeUntilNextToggle ;
    int mSceneDetailIndex ;

//    std::string mDebugText;



    //OIS Input devices
    OIS::InputManager* m_inputManager;
    OIS::Mouse*    m_mouse;
    OIS::Keyboard* m_keyboard;
    OIS::JoyStick* m_joy;
};

#endif // CAMERACONTROLSYSTEMFRAMELISTENER_H
