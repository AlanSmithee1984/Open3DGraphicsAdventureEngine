#ifndef FLOATABLEOBJECT_H
#define FLOATABLEOBJECT_H

#include <OgrePhysXScene.h>
#include "oceanheightobserver.h"
#include "polygon.h"

#include "simpleline.h"
#include "simplelinecollection.h"


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
    Polygons m_polyhedron;

    Ogre::Real m_density;
    Ogre::Real m_untransformedTotalVolume;

    SimpleLine* m_globalLine;
    SimpleLine* m_localLine;
    SimpleLineCollection* m_coll;
};

#endif // FLOATABLEOBJECT_H
