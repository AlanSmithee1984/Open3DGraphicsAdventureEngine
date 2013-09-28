#include "cameracontrolsystemframelistener.h"

CameraControlSystemFrameListener::CameraControlSystemFrameListener()
{
}


bool CameraControlSystemFrameListener::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    return true;
}
