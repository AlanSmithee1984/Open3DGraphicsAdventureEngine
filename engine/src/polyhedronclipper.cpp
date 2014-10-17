#include "polyhedronclipper.h"

#include <OgreRay.h>

#include <QDebug>

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

PolyhedronClipper::PolygonAndIntersectionPoints PolyhedronClipper::clipPolygonAtPlane(const Polygon &subjectPolygon,
                                                                                      const Ogre::Plane &clippingPlane)
{
    Q_ASSERT(subjectPolygon.size() >= 3);

    Polygon inputPolygon = subjectPolygon;
    Polygon outputPolygon;
    QList<Ogre::Vector3> intersectionPoints;

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
                intersectionPoints << intersectionPoint;

                outputPolygon.push_back(intersectionPoint);
            }

            outputPolygon.push_back(pointE);
        }
        else if(sideOfPlaneS == Ogre::Plane::POSITIVE_SIDE)
        {
            Ogre::Vector3 intersectionPoint = calcIntersectionPoint(pointS, pointE, clippingPlane);
            intersectionPoints << intersectionPoint;

            outputPolygon.push_back(intersectionPoint);
        }

        if(sideOfPointE == Ogre::Plane::NO_SIDE)
        {
            // point is exactly on plane
            intersectionPoints << pointE;
        }

        pointS = pointE;
    }

    return PolygonAndIntersectionPoints(outputPolygon, intersectionPoints);

}

bool PolyhedronClipper::less(const VertexWithCenter &first,
                             const VertexWithCenter second)
{
    const Ogre::Vector3 &center = first.second;

    const Ogre::Vector3 &a = first.first;
    const Ogre::Vector3 &b = second.first;

    if (a.x - center.x >= 0 && b.x - center.x < 0)
    {
        return true;
    }

    if (a.x - center.x < 0 && b.x - center.x >= 0)
    {
        return false;
    }

    if (a.x - center.x == 0 && b.x - center.x == 0)
    {
        if (a.y - center.y >= 0 || b.y - center.y >= 0)
        {
            return a.y > b.y;
        }
        else
        {
            return b.y > a.y;
        }
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
    if (det < 0)
        return true;
    if (det > 0)
        return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
    int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
    return d1 > d2;
}

bool PolyhedronClipper::vertexWithCenterComparison(const VertexWithCenter &a,
                                                   const VertexWithCenter &b)
{
    Ogre::Vector3 vec = a.first - b.first;
    Ogre::Real length = vec.squaredLength();
    return length < std::numeric_limits<Ogre::Real>::epsilon();
}

bool PolyhedronClipper::checkCapping(const Polygon &capping, const Ogre::Vector3 &next)
{
    for(quint32 i = 0; i < capping.size(); ++i)
    {
        const Ogre::Vector3 &entry = capping.at(i);
        const Ogre::Vector3 vec = entry - next;
        const Ogre::Real length = vec.squaredLength();

        if(length < std::numeric_limits<Ogre::Real>::epsilon())
        {
            return false;
        }
    }

    return true;
}

void PolyhedronClipper::clipAtPlane(const Polygons &inputFaces, const Ogre::Plane &plane, Polygons &outputFaces)
{
    QList<Ogre::Vector3> intersectionPoints;


    foreach(const Polygon &singleFace, inputFaces)
    {
        PolygonAndIntersectionPoints clippedFace = clipPolygonAtPlane(singleFace, plane);

        if(clippedFace.first.size() >= 3)
        {
            outputFaces << clippedFace.first;
            intersectionPoints << clippedFace.second;
        }
    }


    // form capping polygon
    if(intersectionPoints.empty() == false)
    {
        Ogre::Vector3 center;

        foreach(Ogre::Vector3 intersectionPoint, intersectionPoints)
        {
            center += intersectionPoint;
        }

        center /= intersectionPoints.size();

        std::vector<VertexWithCenter> intersectionPointsTmp;
        foreach(Ogre::Vector3 intersectionPoint, intersectionPoints)
        {
            intersectionPointsTmp.push_back( VertexWithCenter(intersectionPoint, center) );
        }


        std::sort(intersectionPointsTmp.begin(), intersectionPointsTmp.end(), less);

        std::vector<VertexWithCenter>::iterator it = std::unique (intersectionPointsTmp.begin(),
                                                                  intersectionPointsTmp.end(),
                                                                  vertexWithCenterComparison
                                                                  );

        quint32 max = std::distance(intersectionPointsTmp.begin(), it);

        qDebug() << intersectionPointsTmp.size() << max;

        intersectionPointsTmp.resize(max);




        Polygon capping;
        for(quint32 i = 0; i < intersectionPointsTmp.size(); ++i)
        {
            const VertexWithCenter &intersectionPoint = intersectionPointsTmp[i];

            Q_ASSERT(checkCapping(capping, intersectionPoint.first));

            capping.push_back(intersectionPoint.first);
        }

        Q_ASSERT(capping.size() >= 3);
        outputFaces << capping;

    }
}
