#ifndef POLYHEDRONCLIPPER_H
#define POLYHEDRONCLIPPER_H

#include "vertexinformation.h"

class Polyhedron;

class PolyhedronClipper
{
public:
    static void clipAtPlane(const Polyhedron *inputPoly, const Ogre::Plane &plane, VertexInformations &outputVertexInfos);

private:
    PolyhedronClipper();



//    static Ogre::Vector3 calcIntersectionPoint(const Ogre::Vector3 &pointS,
//                                        const Ogre::Vector3 &pointE,
//                                        const Ogre::Plane &clippingPlane);


//    typedef std::pair<Polygon, QList<Edge> > PolygonAndClippingEdges;

//    static PolygonAndClippingEdges clipPolygonAtPlane(const Polygon &subjectPolygon, const Ogre::Plane &clippingPlane);
//    static void constructEdge(const Ogre::Vector3 &intersectionPoint, CheckedEdge &edge, QList<Edge> &edges);
//    static Polygon formCapping(QList<Edge> &edges);

//    static bool existsInPolygon(const Polygon &capping, const Ogre::Vector3 &next);

};

#endif // POLYHEDRONCLIPPER_H
