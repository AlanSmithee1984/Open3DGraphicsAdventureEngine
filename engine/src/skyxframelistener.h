#ifndef SKYXFRAMELISTENER_H
#define SKYXFRAMELISTENER_H

#include <OgreFrameListener.h>

namespace SkyX
{
class BasicController;
class SkyX;
}

class SkyXSettings;

class SkyXFrameListener : public Ogre::FrameListener
{
public:
    SkyXFrameListener(Ogre::SceneManager* sceneManger, Ogre::RenderWindow* window, Ogre::Camera *camera);
    virtual ~SkyXFrameListener();

    virtual bool frameEnded(const Ogre::FrameEvent& evt);

private:
    void setPreset(const SkyXSettings &preset);

    SkyX::BasicController* mBasicController;
    SkyX::SkyX* mSkyX;

    Ogre::Camera* m_camera;
};

#endif // SKYXFRAMELISTENER_H
