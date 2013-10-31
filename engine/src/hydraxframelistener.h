#ifndef HYDRAXFRAMELISTENER_H
#define HYDRAXFRAMELISTENER_H

#include <OgreFrameListener.h>

namespace Hydrax
{
class Hydrax;
}

class HydraxFrameListener : public Ogre::FrameListener
{
public:
    HydraxFrameListener(Ogre::SceneManager* sceneManger, Ogre::Camera* camera);

    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    Hydrax::Hydrax* getHydrax() const;

private:
    void initHydrax(Ogre::SceneManager* sceneManger, Ogre::Camera* camera);
    Hydrax::Hydrax* m_hydrax;
};

#endif // HYDRAXFRAMELISTENER_H
