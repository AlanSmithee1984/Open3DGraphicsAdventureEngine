#ifndef MESHTRIANGLECONVERTER_H
#define MESHTRIANGLECONVERTER_H

#include <OgreMesh.h>

#include "polygon.h"

class MeshTriangleConverter
{
public:
    static Polygons convert(const Ogre::MeshPtr &mesh);

private:
    MeshTriangleConverter();
};

#endif // MESHTRIANGLECONVERTER_H
