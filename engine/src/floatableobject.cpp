#include "floatableobject.h"

#include <Hydrax.h>

#include <QDebug>
#include <QTime>

#include "polyhedronvolumecalculator.h"
#include "polyhedronclipper.h"
#include "polyhedron.h"

FloatableObject::FloatableObject()
    : m_waterHeight(0.0f),
      m_density(0.5f), // FIXME
      m_untransformedTotalVolume(-1.0f),
      m_lineColl(NULL)
{
}

void FloatableObject::oceanHeightUpdated(Hydrax::Hydrax *hydrax)
{
    const Ogre::Vector3 pos = m_coneNode->getPosition();
    m_waterHeight = hydrax->getHeigth(pos);

    if(m_lineColl == NULL)
    {
        m_lineColl = new SimpleLineCollection(hydrax->getSceneManager());
    }

}

void FloatableObject::updateBuoyancy()
{
    QTime timer;
    timer.start();

    const Ogre::Vector3 &objectPos = m_coneNode->getPosition();
    const Ogre::Quaternion &orient = m_coneNode->getOrientation();
    const Ogre::Vector3 &scale = m_coneNode->getScale();


    const Ogre::Vector3 waterPosGlobal(objectPos.x, m_waterHeight, objectPos.z);
    const Ogre::Vector3 waterNormalGlobal(Ogre::Vector3::NEGATIVE_UNIT_Y);

    const Ogre::Plane waterPlaneGlobal(waterNormalGlobal,
                                       waterPosGlobal);



    Ogre::Matrix4 mat(orient);
    mat.setTrans(objectPos);
    mat.setScale(scale);

    mat = mat.inverse();

    Ogre::Plane clippingPlane = mat * waterPlaneGlobal;
    clippingPlane.normal = orient.Inverse() * clippingPlane.normal;


    if(m_untransformedTotalVolume < 0)
    {
        m_untransformedTotalVolume = PolyhedronVolumeCalculator::calcPolyhedronVolume(m_polyhedron->getLocalVertexInformations());
    }

    VertexInformations clippedVertInfo;
    PolyhedronClipper::clipAtPlane(m_polyhedron, clippingPlane, clippedVertInfo);

    const Ogre::Real untransformedClippedVolume = PolyhedronVolumeCalculator::calcPolyhedronVolume(clippedVertInfo);
    const Ogre::Real volumeFraction = untransformedClippedVolume / m_untransformedTotalVolume;

    // could not mass before, because scale could change the resultung mass...
    physx::PxRigidDynamic* actor = m_coneActor.getPxActor();
    const Ogre::Real &transformedTotalMass = actor->getMass();
    const Ogre::Real transformedTotalVolume = transformedTotalMass / m_density;

    const Ogre::Real transformedClippedVolume = transformedTotalVolume * volumeFraction;


    const Ogre::Real fluidDensity = 1.0f; // FIXME
    const Ogre::Real waterMass = transformedClippedVolume * fluidDensity;
    const Ogre::Real buoncyForce = waterMass * 9.81;



    const physx::PxVec3 forceVec(0, buoncyForce, 0);
    actor->addForce(forceVec);




    const physx::PxVec3 objectVelocity = actor->getLinearVelocity();
    const physx::PxVec3 fluidVelocity(0.0f);
    const Ogre::Real referenceArea = 10000; // FIXME

    const Ogre::Real dragCoefficient = 1.0f; // FIXME


    const physx::PxVec3 dragForce = this->calculateDragForce(objectVelocity,
                                                             fluidVelocity,
                                                             referenceArea,
                                                             fluidDensity,
                                                             dragCoefficient);

    actor->addForce(dragForce);


    qDebug() << "volume:" << transformedClippedVolume << waterMass << buoncyForce
             << "\t" << dragForce.x << dragForce.y << dragForce.z
             << "\t" << timer.elapsed();

}

physx::PxVec3 FloatableObject::calculateDragForce(const physx::PxVec3 &objectVelocity,
                                                  const physx::PxVec3 &fluidVelocity,
                                                  const Ogre::Real &referenceArea,
                                                  const Ogre::Real &density,
                                                  const Ogre::Real &dragCoefficient)
{
    const Ogre::Real factor = 0.5f * density * dragCoefficient * referenceArea;

    const physx::PxVec3 relativeObjectVel = objectVelocity - fluidVelocity;
    physx::PxVec3 squaredObjVal(relativeObjectVel.x * relativeObjectVel.x,
                                relativeObjectVel.y * relativeObjectVel.y,
                                relativeObjectVel.z * relativeObjectVel.z);

    if(std::signbit(relativeObjectVel.x) == false)
    {
        squaredObjVal.x = -squaredObjVal.x;
    }

    if(std::signbit(relativeObjectVel.y) == false)
    {
        squaredObjVal.y = -squaredObjVal.y;
    }

    if(std::signbit(relativeObjectVel.z) == false)
    {
        squaredObjVal.z = -squaredObjVal.z;
    }

    const physx::PxVec3 dragForce = squaredObjVal * factor;

    return dragForce;
}
