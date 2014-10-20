#include "floatableobject.h"

#include <Hydrax.h>

#include <QDebug>

#include "polyhedronvolumecalculator.h"
#include "polyhedronclipper.h"

FloatableObject::FloatableObject()
    : m_waterHeight(0.0f)
{
}

void FloatableObject::oceanHeightUpdated(Hydrax::Hydrax *hydrax)
{
    const Ogre::Vector3 pos = m_coneNode->getPosition();
    m_waterHeight = hydrax->getHeigth(pos);
}

void FloatableObject::updateBuoyancy()
{

    const Ogre::Vector3 pos = m_coneNode->getPosition();


    Ogre::Vector3 planeNormal(Ogre::Vector3::UNIT_Y);


    const Ogre::Quaternion &orient = m_coneNode->getOrientation();
    const Ogre::Vector3 &scale = m_coneNode->getScale();

    Ogre::Matrix4 mat(orient);
    mat.setTrans(pos);
    mat.setScale(scale);

    mat = mat.inverse();

    Ogre::Vector3 waterPos(pos.x, m_waterHeight, pos.z);

    waterPos = mat * waterPos;
    planeNormal = orient * planeNormal;

    const Ogre::Plane clippingPlane(-planeNormal, waterPos);

//    mat = mat.inverse();
//    Ogre::Vector3 start = mat * waterPos;
//    Ogre::Vector3 end = start + clippingPlane.normal * clippingPlane.d;
    //    end = Ogre::Vector3::ZERO;
    //    end.y = 1000;
//    SimpleLine::LineAttributes attr(start, end, Ogre::ColourValue::Red, Ogre::ColourValue::Blue );
//    m_line.setLineData(attr);


    Polygons cappedPoly;
    PolyhedronClipper::clipAtPlane(m_polys, clippingPlane, cappedPoly);

    //    std::cout << clippingPlane << "\t" << m_polys.size() << "\t"  << cappedPoly.size() << std::endl;

    Ogre::Real volume = PolyhedronVolumeCalculator::calcPolyhedronVolume(cappedPoly);



    Ogre::Real mass = volume * 1.0;
    Ogre::Real force = mass * 9.81;

    force *= 100000;


    qDebug() << "volume:" << volume << mass << force << m_coneActor.getPxActor()->getMass();

    physx::PxVec3 forceVec(0, force, 0);

    m_coneActor.getPxActor()->addForce(forceVec);

}
