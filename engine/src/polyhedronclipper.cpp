#include "polyhedronclipper.h"

#include <OgreRay.h>

#include <QDebug>

#include "pointsorter.h"

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


bool PolyhedronClipper::vertexWithCenterComparison(const VertexSortingInformation &a,
                                                   const VertexSortingInformation &b)
{
    Ogre::Vector3 vec = a.point - b.point;

    bool isZeroLength = vec.isZeroLength();

    if(isZeroLength)
    {
        qDebug() << isZeroLength;
    }

    return isZeroLength;
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

        std::vector<VertexSortingInformation> intersectionPointsTmp;
        foreach(Ogre::Vector3 intersectionPoint, intersectionPoints)
        {
            intersectionPointsTmp.push_back( VertexSortingInformation(intersectionPoint, center, plane.normal) );
        }

        std::cout << "unsorted" << std::endl;
        foreach(VertexSortingInformation intersectionPoint, intersectionPointsTmp)
        {
            std::cout << intersectionPoint.point << std::endl;
        }

        PointSorter::sortPoints(intersectionPointsTmp);

        std::cout << "sorted" << std::endl;
        foreach(VertexSortingInformation intersectionPoint, intersectionPointsTmp)
        {
            std::cout << intersectionPoint.point << std::endl;
        }

        std::vector<VertexSortingInformation>::iterator it = std::unique (intersectionPointsTmp.begin(),
                                                                          intersectionPointsTmp.end(),
                                                                          vertexWithCenterComparison
                                                                          );


        quint32 max = std::distance(intersectionPointsTmp.begin(), it);

        qDebug() << intersectionPointsTmp.size() << max;

        intersectionPointsTmp.resize(max);


        std::cout << "unique" << std::endl;
        foreach(VertexSortingInformation intersectionPoint, intersectionPointsTmp)
        {
            std::cout << intersectionPoint.point << std::endl;
        }



        Polygon capping;
        for(quint32 i = 0; i < intersectionPointsTmp.size(); ++i)
        {
            const VertexSortingInformation &intersectionPoint = intersectionPointsTmp[i];

            const Ogre::Vector3& p = intersectionPoint.point;

            Q_ASSERT(existsInPolygon(capping, p) == false);

            capping.push_back(p);
        }

        Q_ASSERT(capping.size() >= 3);
        outputFaces << capping;

    }
}
