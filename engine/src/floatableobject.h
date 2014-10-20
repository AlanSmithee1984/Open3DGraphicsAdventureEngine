#ifndef FLOATABLEOBJECT_H
#define FLOATABLEOBJECT_H

#include <OgrePhysXScene.h>
#include "oceanheightobserver.h"
#include "polygon.h"

#include "simpleline.h"


namespace OgrePhysX
{
    class Scene;
}


class FloatableObject : public OceanHeightObserver
{
public:
    FloatableObject();

    virtual void oceanHeightUpdated(Hydrax::Hydrax *hydrax);
    void updateBuoyancy();

protected:
    Ogre::Real m_waterHeight;
    Ogre::SceneNode* m_coneNode;
    OgrePhysX::Actor<physx::PxRigidDynamic> m_coneActor;
    Polygons m_polys;

//    SimpleLine m_line;
};

#endif // FLOATABLEOBJECT_H
