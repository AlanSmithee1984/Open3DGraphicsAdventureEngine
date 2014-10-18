#ifndef VERTEXSORTINGINFORMATION_H
#define VERTEXSORTINGINFORMATION_H

#include <OgreVector3.h>

struct VertexSortingInformation
{
    VertexSortingInformation();

    VertexSortingInformation(const VertexSortingInformation &other);

    VertexSortingInformation(const Ogre::Vector3 &p,
                             const Ogre::Vector3 &c,
                             const Ogre::Vector3 &n);

    Ogre::Vector3 point;
    Ogre::Vector3 center;
    Ogre::Vector3 normal;
};

#endif // VERTEXSORTINGINFORMATION_H
