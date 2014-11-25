#ifndef MESHTRIANGLECONVERTER_H
#define MESHTRIANGLECONVERTER_H

#include <OgreMesh.h>

class Polyhedron;

class MeshTriangleConverter
{
public:
    static Polyhedron* convert(const Ogre::MeshPtr &mesh);

private:
    MeshTriangleConverter();
};

#endif // MESHTRIANGLECONVERTER_H
