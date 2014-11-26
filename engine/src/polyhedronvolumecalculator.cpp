#include "polyhedronvolumecalculator.h"

#include <OgrePlane.h>

#include <QDebug>

#include "vertexinformation.h"

Ogre::Real PolyhedronVolumeCalculator::calcPolygonArea(const Polygon &face, const Ogre::Vector3 &planeNormal)
{
    Q_ASSERT(face.size() >= 3);

    Ogre::Vector3 sumCross(0.0f);

    for(quint32 i = 0; i < face.size(); ++i)
    {
        Ogre::Vector3 vertexCurrent = face[i];

        quint32 nextIndex = (i + 1) % face.size();
        Ogre::Vector3 vertexNext = face[nextIndex];

        Ogre::Vector3 cross = vertexCurrent.crossProduct(vertexNext);
        sumCross += cross;
    }


    Q_ASSERT(planeNormal.isZeroLength() == false);

    const Ogre::Real area = planeNormal.dotProduct(sumCross) * 0.5;

    return area;
}

Ogre::Real PolyhedronVolumeCalculator::calcPolyhedronVolume(const VertexInformations &vertices)
{
    Ogre::Real sum = 0;
    foreach(const VertexInformation* info, vertices)
    {
        Ogre::Real val = info->calcVertexVolumeEquation();

        qDebug() << val;
        Q_ASSERT(val >= 0);

        sum += val;
    }

    return sum;
}

Ogre::Real PolyhedronVolumeCalculator::calcPolyhedronVolume(const Polygons &faces)
{
    Ogre::Real vol = 0;


    foreach(Polygon face, faces)
    {

        // assume all points on the same plane
        const Ogre::Plane plane(face[0], face[1], face[2]);

        const Ogre::Real singleArea = calcPolygonArea(face, plane.normal);

        const Ogre::Real height = plane.getDistance(Ogre::Vector3::ZERO);

        const Ogre::Real singleVolume = singleArea * height;

//        qDebug() << plane.normal.x
//                 << "\t" << plane.normal.y
//                 << "\t" << plane.normal.z
//                 << "\t" << plane.d
//                 << "\t" << singleArea
//                 << "\t" << height
//                 << "\t" << singleVolume;

        vol += singleVolume;

    }

    vol /= 3;


    return vol;
}

PolyhedronVolumeCalculator::PolyhedronVolumeCalculator()
{
}

