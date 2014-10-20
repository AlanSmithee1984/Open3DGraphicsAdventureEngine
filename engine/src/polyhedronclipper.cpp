#include "polyhedronclipper.h"

#include <OgreRay.h>

#include <QDebug>

#include "edgesorter.h"
#include "checkededge.h"

PolyhedronClipper::PolyhedronClipper()
{
}

Ogre::Vector3 PolyhedronClipper::calcIntersectionPoint(const Ogre::Vector3 &pointS, const Ogre::Vector3 &pointE,
                                                       const Ogre::Plane &clippingPlane)
{
    Ogre::Ray ray(pointS, pointE - pointS);
    std::pair<bool, Ogre::Real> intersection = ray.intersects(clippingPlane);

    Q_ASSERT(intersection.first);

    Ogre::Vector3 intersectionPoint = ray.getPoint(intersection.second);

    return intersectionPoint;
}

PolyhedronClipper::PolygonAndClippingEdges PolyhedronClipper::clipPolygonAtPlane(const Polygon &subjectPolygon,
                                                                                      const Ogre::Plane &clippingPlane)
{
    Q_ASSERT(subjectPolygon.size() >= 3);

    Polygon inputPolygon = subjectPolygon;
    Polygon outputPolygon;

    QList<Edge> intersectionEdges;
    CheckedEdge tmpEdge;


    Ogre::Vector3 pointS = inputPolygon.back();
    foreach(Ogre::Vector3 pointE, inputPolygon)
    {
        const Ogre::Plane::Side sideOfPointE = clippingPlane.getSide(pointE);
        const Ogre::Plane::Side sideOfPlaneS = clippingPlane.getSide(pointS);

        if(sideOfPointE == Ogre::Plane::POSITIVE_SIDE)
        {
            // point E is inside of clipping plane

            if(sideOfPlaneS != Ogre::Plane::POSITIVE_SIDE)
            {
                // in to out situation
                Ogre::Vector3 intersectionPoint = calcIntersectionPoint(pointS, pointE, clippingPlane);

                constructEdge(intersectionPoint, tmpEdge, intersectionEdges);

                outputPolygon.push_back(intersectionPoint);
            }

            outputPolygon.push_back(pointE);
        }
        else if(sideOfPlaneS == Ogre::Plane::POSITIVE_SIDE)
        {
            Ogre::Vector3 intersectionPoint = calcIntersectionPoint(pointS, pointE, clippingPlane);
            constructEdge(intersectionPoint, tmpEdge, intersectionEdges);

            outputPolygon.push_back(intersectionPoint);
        }

//        if(sideOfPointE == Ogre::Plane::NO_SIDE)
//        {
//            // point is exactly on plane
//            constructEdge(pointE, tmpEdge, intersectionEdges);
//        }

        pointS = pointE;
    }

    return PolygonAndClippingEdges(outputPolygon, intersectionEdges);

}

void PolyhedronClipper::constructEdge(const Ogre::Vector3 &intersectionPoint,
                                      CheckedEdge &edge, QList<Edge> &edges)
{
    if(edge.start.first == false)
    {
        edge.start.first = true;
        edge.start.second = intersectionPoint;
    }
    else if(edge.end.first == false)
    {
        edge.end.first = true;
        edge.end.second = intersectionPoint;
    }

    if(edge.start.first == true && edge.end.first == true)
    {
        edges << Edge(edge.start.second, edge.end.second);
        edge.reset();
    }

}

Polygon PolyhedronClipper::formCapping(QList<Edge> &edges)
{
    std::cout << "unsorted" << std::endl;
    foreach(const Edge &edge, edges)
    {
        std::cout << edge  << std::endl;
    }

    Polygon capping = EdgeSorter::sortEdges(edges);

    std::cout << "sorted" << std::endl;
    foreach(const Ogre::Vector3 &point, capping)
    {
        std::cout << point << std::endl;
    }


    return capping;
}

bool PolyhedronClipper::existsInPolygon(const Polygon &capping, const Ogre::Vector3 &next)
{
    for(quint32 i = 0; i < capping.size(); ++i)
    {
        const Ogre::Vector3 &entry = capping.at(i);
        const Ogre::Vector3 vec = entry - next;
        bool isZeroLength = vec.isZeroLength();

        if(isZeroLength)
        {
            return true;
        }
    }

    return false;
}

void PolyhedronClipper::clipAtPlane(const Polygons &inputFaces, const Ogre::Plane &plane, Polygons &outputFaces)
{
    QList<Edge> intersectionEdges;

    foreach(const Polygon &singleFace, inputFaces)
    {
        PolygonAndClippingEdges clippedFace = clipPolygonAtPlane(singleFace, plane);

        if(clippedFace.first.size() >= 3)
        {
            outputFaces << clippedFace.first;

            Q_ASSERT(clippedFace.second.size() == 1 || clippedFace.second.size() == 0);
            intersectionEdges << clippedFace.second;
        }
    }


    // form capping polygon


    if(intersectionEdges.empty() == false)
    {
        Polygon cappingPoly = formCapping(intersectionEdges);

        Q_ASSERT(cappingPoly.size() >= 3);
        outputFaces << cappingPoly;

    }

}
