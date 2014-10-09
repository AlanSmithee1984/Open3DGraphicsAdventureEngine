#ifndef SKYXFRAMELISTENER_H
#define SKYXFRAMELISTENER_H

#include <OgreFrameListener.h>
#include <SkyX.h>

namespace Hydrax
{
class Hydrax;
}

class SkyXSettings;

class SkyXFrameListener : public Ogre::FrameListener
{
public:
    SkyXFrameListener(Ogre::SceneManager* sceneManger,
                      Ogre::RenderWindow* window,
                      Ogre::Camera *camera);

    virtual ~SkyXFrameListener();

    void setHydrax(Hydrax::Hydrax *hydrax);

    virtual bool frameStarted(const Ogre::FrameEvent &e);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    SkyX::SkyX *getSkyX() const;
    Ogre::Light *getSunLight() const;

private:
    void setPreset(const SkyXSettings &preset);
    void setColorGradients();

    /** Update environment lighting
     */
    void updateEnvironmentLighting();

    /** Update shadow far distance
     */
    void updateShadowFarDistance();


    Ogre::Camera* m_camera;
    bool mForceDisableShadows;

    SkyX::SkyX* m_skyX;
    SkyX::BasicController* mBasicController;
    Hydrax::Hydrax* m_hydrax;


    // Light
    Ogre::Light *m_sunLight;
    // Shadow caster
    Ogre::Light *m_shadowLight;

    Ogre::Real mLastPositionLength;

    // Color gradients
    SkyX::ColorGradient mWaterGradient,
                        mSunGradient,
                        mAmbientGradient;


};

#endif // SKYXFRAMELISTENER_H
