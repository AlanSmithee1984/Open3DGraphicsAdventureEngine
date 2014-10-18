#include "vertexsortinginformation.h"


VertexSortingInformation::VertexSortingInformation()
    : point(Ogre::Vector3::ZERO),
      center(Ogre::Vector3::ZERO),
      normal(Ogre::Vector3::ZERO)
{

}

VertexSortingInformation::VertexSortingInformation(const VertexSortingInformation &other)
    : point(other.point),
      center(other.center),
      normal(other.normal)
{

}

VertexSortingInformation::VertexSortingInformation(const Ogre::Vector3 &p, const Ogre::Vector3 &c, const Ogre::Vector3 &n)
    : point(p),
      center(c),
      normal(n)
{

}
