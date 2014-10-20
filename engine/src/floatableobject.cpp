#include "floatableobject.h"

#include <Hydrax.h>

#include <QDebug>

#include "polyhedronvolumecalculator.h"
#include "polyhedronclipper.h"

FloatableObject::FloatableObject()
    : m_waterHeight(0.0f),
      m_density(0.1f), // FIXME
      m_untransformedTotalVolume(-1.0f)
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


    Polygons cappedPolyhedron;
    PolyhedronClipper::clipAtPlane(m_polyhedron, clippingPlane, cappedPolyhedron);

    //    std::cout << clippingPlane << "\t" << m_polys.size() << "\t"  << cappedPoly.size() << std::endl;

    if(m_untransformedTotalVolume < 0)
    {
        m_untransformedTotalVolume = PolyhedronVolumeCalculator::calcPolyhedronVolume(m_polyhedron);
    }

    physx::PxRigidDynamic* actor = m_coneActor.getPxActor();

    const Ogre::Real untransformedVolume = PolyhedronVolumeCalculator::calcPolyhedronVolume(cappedPolyhedron);
    const Ogre::Real volumeFraction = untransformedVolume / m_untransformedTotalVolume;

    const Ogre::Real &transformedTotalMass = actor->getMass();
    const Ogre::Real transformedTotalVolume = transformedTotalMass / m_density;

    const Ogre::Real transformedClippedVolume = transformedTotalVolume * volumeFraction;



    Ogre::Real mass = transformedClippedVolume * 1.0;
    Ogre::Real force = mass * 9.81;

//    force *= 0.1;

    physx::PxVec3 vel = actor->getLinearVelocity();

    if(vel.y < 0 && waterPos.y > -2)
    {
        // downwards
        // damp it

        qDebug() << "damping" << vel.y;

        vel.y = 0.99 * vel.y;


    }

    actor->setLinearVelocity(vel);



    qDebug() << "volume:" << transformedClippedVolume << mass << force << waterPos.y ;

    physx::PxVec3 forceVec(0, force, 0);

    actor->addForce(forceVec);

}
