#include "cone.h"

#include <OgrePhysXNodeRenderable.h>
#include <OgreSceneManager.h>

#include "meshtriangleconverter.h"

std::vector<OgrePointWithNormal> generateCirclePoints()
{
    const Ogre::Real circleTemplateRadius = 1.0;

    const Ogre::Real twoPI = Ogre::Math::PI + Ogre::Math::PI;

    const quint32 accuracy = 4;

    const Ogre::Real stepAngle = twoPI / accuracy;

    quint32 numSteps = twoPI / stepAngle;

    std::vector<OgrePointWithNormal> templatePoints;
    templatePoints.reserve(numSteps);

    Ogre::Real theta = 0;
    for(quint32 index = 0; index < numSteps; theta -= stepAngle, ++index)
    {
        OgrePointWithNormal pointWithNormal;
        pointWithNormal.point.x = circleTemplateRadius * cos(-theta);
        pointWithNormal.point.z = circleTemplateRadius * sin(-theta);
        pointWithNormal.point.y = 0.0f;

        pointWithNormal.normal = pointWithNormal.point - Ogre::Vector3::ZERO;

        templatePoints.push_back(pointWithNormal);
    }

    return templatePoints;
}

std::vector<OgrePointWithNormal> Cone::s_templatePoints = generateCirclePoints();

Cone::Cone(Ogre::SceneManager* pSceneManager, OgrePhysX::Scene* physXScene)
{
    Ogre::ManualObject* coneMO = pSceneManager->createManualObject();



    const Ogre::Real radius = 1;

    const Ogre::Vector3 tip(0, 1, 0);

    //    Ogre::ColourValue col(0.5, 0.5, 0.5, 0.5);

    coneMO->begin("rock");


    coneMO->position(Ogre::Vector3::ZERO);
    coneMO->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);

    coneMO->position(tip);
    coneMO->normal(Ogre::Vector3::UNIT_Y);

    Q_ASSERT(s_templatePoints.empty() == false);

    const OgrePointWithNormal& p = s_templatePoints[0];
    coneMO->position(p.point * radius);
    coneMO->normal(p.normal);

    const quint32 startIndex = 3;
    quint32 currIndex = startIndex;

    for(quint32 i = 1; i < s_templatePoints.size() ; ++i)
    {
        const OgrePointWithNormal& p = s_templatePoints[i];

        coneMO->position(p.point * radius);
        coneMO->normal(p.normal);

        // bottom
        coneMO->triangle(0, currIndex - 1, currIndex);

        // side face
        coneMO->triangle(1, currIndex - 1, currIndex);

        ++currIndex;
    }

    // bottom
    coneMO->triangle(0, currIndex - 1, startIndex);

    // side face
    coneMO->triangle(1, currIndex - 1, startIndex);



    coneMO->end();


    Ogre::MeshPtr coneMesh = coneMO->convertToMesh("coneMesh");

    Ogre::Entity* coneEnt = pSceneManager->createEntity(coneMesh);
    m_coneNode = pSceneManager->getRootSceneNode()->createChildSceneNode();
    m_coneNode->attachObject(coneEnt);

    m_coneNode->setVisible(true);



    const Ogre::Vector3 conePos(0, 500, 1500);
    const Ogre::Vector3 coneScale(50.0f);

    m_coneNode->setScale(coneScale);
    m_coneNode->setPosition(conePos);
    m_coneNode->showBoundingBox(true);

//    //create physical actor
//    m_coneActor = physXScene->createRigidDynamic(coneEnt, m_density, coneScale);

//    //    setupFiltering(fish1Actor.getPxActor(), eFish, eAll);

//    //    physx::PxVec3 vel1(0, 50, 0);
//    //    fish1Actor.getPxActor()->setLinearVelocity(vel1);


//    //setup binding
//    physXScene->createRenderedActorBinding(m_coneActor, new OgrePhysX::NodeRenderable(m_coneNode));

//    m_coneActor.setGlobalPosition(conePos);

    //    Ogre::Quaternion rot;
    //    rot.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_X);

    //    m_coneActor.setGlobalOrientation(rot);


    m_polyhedron = MeshTriangleConverter::convert(coneMesh);


}


















