#ifndef PHYSXHELPER_H
#define PHYSXHELPER_H

#include <PxVec3.h>
#include <OgreVector3.h>

class PhysxHelper
{
public:

    static Ogre::Vector3 convertToOgreVector(const physx::PxVec3 &vec);
    static Ogre::ColourValue convertToOgreColour(const physx::PxU32 &colorVal);


private:
    PhysxHelper();
};

#endif // PHYSXHELPER_H
