#ifndef POLYHEDRONVOLUMECALCULATOR_H
#define POLYHEDRONVOLUMECALCULATOR_H

#include "polygon.h"

class VertexInformation;

class PolyhedronVolumeCalculator
{
public:

    static Ogre::Real calcPolyhedronVolume(const Polygons &faces);
    static Ogre::Real calcPolygonArea(const Polygon &face,
                                      const Ogre::Vector3 &planeNormal);

    static Ogre::Real calcPolyhedronVolume(const QList<VertexInformation*> &vertices);

private:
    PolyhedronVolumeCalculator();


};

#endif // POLYHEDRONVOLUMECALCULATOR_H
