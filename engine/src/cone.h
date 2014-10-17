#ifndef CONE_H
#define CONE_H

#include <OgrePhysXScene.h>
#include "oceanheightobserver.h"
#include "polygon.h"

#include "simpleline.h"

namespace Ogre
{
    class SceneManager;
}

namespace OgrePhysX
{
    class Scene;
}

class Cone : public OceanHeightObserver
{
public:
    Cone(Ogre::SceneManager *pSceneManager, OgrePhysX::Scene *physXScene);

    virtual void oceanHeightUpdated(Hydrax::Hydrax *hydrax);

private:
    Ogre::SceneNode* m_coneNode;
    OgrePhysX::Actor<physx::PxRigidDynamic> m_coneActor;
    Polygons m_polys;

    SimpleLine m_line;

};

#endif // CONE_H
