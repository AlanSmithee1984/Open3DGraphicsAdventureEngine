#include "floatableobject.h"

#include <Hydrax.h>

#include <QDebug>

#include "polyhedronvolumecalculator.h"
#include "polyhedronclipper.h"

FloatableObject::FloatableObject()
    : m_waterHeight(0.0f),
      m_density(0.5f), // FIXME
      m_untransformedTotalVolume(-1.0f),
      m_globalLine(NULL),
      m_localLine(NULL),
      m_coll(NULL)
{
}

void FloatableObject::oceanHeightUpdated(Hydrax::Hydrax *hydrax)
{
    const Ogre::Vector3 pos = m_coneNode->getPosition();
    m_waterHeight = hydrax->getHeigth(pos);

    if(m_globalLine == NULL)
    {
        m_globalLine = new SimpleLine(hydrax->getSceneManager());
        m_localLine = new SimpleLine(hydrax->getSceneManager());
        m_coll = new SimpleLineCollection(hydrax->getSceneManager());
    }

}

void FloatableObject::updateBuoyancy()
{
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



//    // transform global water position to model local space
//    const Ogre::Vector3 waterPosLocal = mat * waterPosGlobal;
//    const Ogre::Vector3 waterNormalLocal = orient.Inverse() * waterNormalGlobal;



//    const Ogre::Plane clippingPlane(-waterNormalLocal, waterPosLocal);

    //    std::cout << clippingPlane.normal << "\t" << clippingPlane.d << std::endl;



//    Ogre::Vector3 startGlobal = waterPosGlobal;
//    Ogre::Vector3 endGlobal = startGlobal + waterNormalGlobal * 100;
//    SimpleLine::LineAttributes attrGlobal(startGlobal, endGlobal, Ogre::ColourValue::Blue,
//                                          Ogre::ColourValue::Red );
//    m_globalLine->setLineData(attrGlobal);


//    mat = mat.inverse();

//    Ogre::Vector3 startLocal = mat * waterPosLocal;
//    Ogre::Vector3 endLocal = startLocal + mat.extractQuaternion() * waterNormalLocal * 100;
//    SimpleLine::LineAttributes attrLocal(startLocal, endLocal, Ogre::ColourValue::Green,
//                                         Ogre::ColourValue::Red );
//    m_localLine->setLineData(attrLocal);



    //    std::cout << clippingPlane << "\t" << m_polys.size() << "\t"  << cappedPoly.size() << std::endl;

    if(m_untransformedTotalVolume < 0)
    {
        m_untransformedTotalVolume = PolyhedronVolumeCalculator::calcPolyhedronVolume(m_polyhedron);
    }



    Polygons cappedPolyhedron;
    PolyhedronClipper::clipAtPlane(m_polyhedron, clippingPlane, cappedPolyhedron);



    QList<SimpleLine::LineAttributes> attributes;

//    std::cout << orient << std::endl;

    Ogre::Matrix4 debugMat(orient);
    debugMat.setTrans(objectPos);
    debugMat.setScale(scale);

    foreach(Polygon poly, cappedPolyhedron)
    {
        //        if(cappedPolyhedron.isEmpty() == false)
        //        {
        //            Polygon poly = cappedPolyhedron.last();

        Q_ASSERT(poly.size() >= 3);


        Ogre::Vector3 lastVertex = debugMat * (orient *  poly[0]);

        Ogre::Vector3 vertex = debugMat * (orient *  poly.back());

        SimpleLine::LineAttributes attr(lastVertex, vertex,
                                        Ogre::ColourValue::Red,
                                        Ogre::ColourValue::Red);

        attributes << attr;


        for(quint32 i = 1; i < poly.size(); ++i)
        {
            Ogre::Vector3 vertex = debugMat * (orient *  poly[i]);

            SimpleLine::LineAttributes attr(lastVertex, vertex,
                                            Ogre::ColourValue::Red,
                                            Ogre::ColourValue::Red);

            attributes << attr;

            lastVertex = vertex;
        }


        //        }
    }

    m_coll->setLineData(attributes);

    const Ogre::Real untransformedClippedVolume = PolyhedronVolumeCalculator::calcPolyhedronVolume(cappedPolyhedron);
    const Ogre::Real volumeFraction = untransformedClippedVolume / m_untransformedTotalVolume;

    // could not mass before, because scale could change the resultung mass...
    physx::PxRigidDynamic* actor = m_coneActor.getPxActor();
    const Ogre::Real &transformedTotalMass = actor->getMass();
    const Ogre::Real transformedTotalVolume = transformedTotalMass / m_density;

    const Ogre::Real transformedClippedVolume = transformedTotalVolume * volumeFraction;



    Ogre::Real mass = transformedClippedVolume * 1.0;
    Ogre::Real force = mass * 9.81;

    //    force *= 0.1;

    //    physx::PxVec3 vel = actor->getLinearVelocity();

    //    if(vel.y < 0 && waterPos.y > -2)
    //    {
    //        // downwards
    //        // damp it

    //        qDebug() << "damping" << vel.y;

    //        vel.y = 0.99 * vel.y;


    //    }

    //    actor->setLinearVelocity(vel);



    qDebug() << "volume:" << transformedClippedVolume << mass << force;

    physx::PxVec3 forceVec(0, force, 0);

    actor->addForce(forceVec);

}
