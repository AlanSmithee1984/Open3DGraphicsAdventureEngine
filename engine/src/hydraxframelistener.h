#ifndef HYDRAXFRAMELISTENER_H
#define HYDRAXFRAMELISTENER_H

#include <OgreFrameListener.h>

#include <QList>

namespace Hydrax
{
class Hydrax;
}

class OceanHeightObserver;

class HydraxFrameListener : public Ogre::FrameListener
{
public:
    HydraxFrameListener(Ogre::SceneManager* sceneManger, Ogre::Camera* camera);

    virtual bool frameStarted(const Ogre::FrameEvent& evt);

    Hydrax::Hydrax* getHydrax() const;

    void addHeightObserver(OceanHeightObserver* observer);

private:
    Hydrax::Hydrax* m_hydrax;

    QList<OceanHeightObserver*> m_oceanHeigthObservers;
};

#endif // HYDRAXFRAMELISTENER_H
