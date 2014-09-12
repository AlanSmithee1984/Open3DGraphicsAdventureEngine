#include "physxhelper.h"

#include <OgreColourValue.h>

PhysxHelper::PhysxHelper()
{
}

Ogre::Vector3 PhysxHelper::convertToOgreVector(const physx::PxVec3 &vec)
{
    Ogre::Vector3 out(vec.x, vec.y, vec.z);

    return out;
}

Ogre::ColourValue PhysxHelper::convertToOgreColour(const physx::PxU32 &colorVal)
{
    Ogre::ColourValue outCol;

    Ogre::RGBA rgba = colorVal;

    outCol.setAsRGBA(rgba);

    return outCol;
}
