#ifndef SKYXFRAMELISTENER_H
#define SKYXFRAMELISTENER_H

#include <OgreFrameListener.h>

namespace SkyX
{
class BasicController;
class SkyX;
}

namespace Hydrax
{
class Hydrax;
}

class SkyXSettings;

class SkyXFrameListener : public Ogre::FrameListener
{
public:
    SkyXFrameListener(Ogre::SceneManager* sceneManger, Ogre::RenderWindow* window, Ogre::Camera *camera);
    virtual ~SkyXFrameListener();

    virtual bool frameStarted(const Ogre::FrameEvent &e);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

private:
    void setPreset(const SkyXSettings &preset);

    /** Update environment lighting
     */
    void updateEnvironmentLighting();

    /** Update shadow far distance
     */
    void updateShadowFarDistance();

    SkyX::BasicController* mBasicController;
    SkyX::SkyX* mSkyX;
    Hydrax::Hydrax* m_hydrax;

    Ogre::Camera* m_camera;
    bool mForceDisableShadows;
};

#endif // SKYXFRAMELISTENER_H
