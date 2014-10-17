#ifndef POLYHEDRONCLIPPER_H
#define POLYHEDRONCLIPPER_H

#include "polygon.h"

typedef std::pair<Ogre::Vector3, Ogre::Vector3> VertexWithCenter;

class PolyhedronClipper
{
public:
    static void clipAtPlane(const Polygons &inputFaces, const Ogre::Plane &plane, Polygons &outputFaces);

private:
    PolyhedronClipper();



    static Ogre::Vector3 calcIntersectionPoint(const Ogre::Vector3 &pointS,
                                        const Ogre::Vector3 &pointE,
                                        const Ogre::Plane &clippingPlane);

    typedef std::pair<Polygon, QList<Ogre::Vector3> > PolygonAndIntersectionPoints;

    static PolygonAndIntersectionPoints clipPolygonAtPlane(const Polygon &subjectPolygon, const Ogre::Plane &clippingPlane);




    static bool less(const VertexWithCenter & first, const VertexWithCenter second);

    static bool vertexWithCenterComparison(const VertexWithCenter &a, const VertexWithCenter &b);

    static bool checkCapping(const Polygon &capping, const Ogre::Vector3 &next);

};

#endif // POLYHEDRONCLIPPER_H
