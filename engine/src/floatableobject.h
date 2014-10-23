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
    physx::PxVec3 calculateDragForce(const physx::PxVec3 &objectVelocity,
                                  const physx::PxVec3 &fluidVelocity,
                                  const Ogre::Real &referenceArea,
                                  const Ogre::Real &density,
                                  const Ogre::Real &dragCoefficient);

    Ogre::Real m_waterHeight;
    Ogre::SceneNode* m_coneNode;
    OgrePhysX::Actor<physx::PxRigidDynamic> m_coneActor;
    Polygons m_polyhedron;

    Ogre::Real m_density;
    Ogre::Real m_untransformedTotalVolume;

    SimpleLineCollection* m_lineColl;
};

#endif // FLOATABLEOBJECT_H
