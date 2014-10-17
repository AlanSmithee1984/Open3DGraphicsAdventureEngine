#include "polyhedronvolumecalculator.h"

#include <OgrePlane.h>

#include <QDebug>

Ogre::Real PolyhedronVolumeCalculator::calcPolyhedronVolume(const Polygons &faces)
{
    Ogre::Real vol = 0;


    foreach(Polygon face, faces)
    {
        Q_ASSERT(face.size() >= 3);

        Ogre::Vector3 sumCross(0.0f);

        for(quint32 i = 0; i < face.size(); ++i)
        {
            Ogre::Vector3 vertexCurrent = face[i];

            quint32 nextIndex = (i + 1) % face.size();
            Ogre::Vector3 vertexNext    = face[nextIndex];

            Ogre::Vector3 cross = vertexCurrent.crossProduct(vertexNext);
            sumCross += cross;
        }

        const Ogre::Plane plane(face[0], face[1], face[2]);

        Q_ASSERT(plane.normal.isZeroLength() == false);

        const Ogre::Real singleArea = plane.normal.dotProduct(sumCross) * 0.5;


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
