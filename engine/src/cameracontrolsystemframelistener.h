#ifndef CAMERACONTROLSYSTEMFRAMELISTENER_H
#define CAMERACONTROLSYSTEMFRAMELISTENER_H

#include <OgreFrameListener.h>

class CameraControlSystemFrameListener: public Ogre::FrameListener
{
public:
    CameraControlSystemFrameListener();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // CAMERACONTROLSYSTEMFRAMELISTENER_H
