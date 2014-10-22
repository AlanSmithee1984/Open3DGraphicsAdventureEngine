#include "cone.h"

#include <OgrePhysXNodeRenderable.h>
#include <OgreSceneManager.h>

#include "meshtriangleconverter.h"


Cone::Cone(Ogre::SceneManager* pSceneManager, OgrePhysX::Scene* physXScene)
{
    Ogre::ManualObject* coneMO = pSceneManager->createManualObject();



    const Ogre::Real baseHeight = -2;

    Ogre::Vector3 lowerLeftBack(-2, baseHeight, 2);
    Ogre::Vector3 lowerRightBack(2, baseHeight, 2);
    Ogre::Vector3 lowerRightFront(2, baseHeight, -2);
    Ogre::Vector3 lowerLeftFront(-2, baseHeight, -2);

    Ogre::Vector3 tip(-2, 2, -2);

//    Ogre::ColourValue col(0.5, 0.5, 0.5, 0.5);

    coneMO->begin("rock");

    coneMO->position(lowerLeftBack);
    coneMO->normal(lowerLeftBack - lowerRightFront);

    coneMO->position(lowerRightBack);
    coneMO->normal(lowerRightBack - lowerLeftFront);

    coneMO->position(lowerRightFront);
    coneMO->normal(lowerRightFront - lowerLeftBack);

    coneMO->position(lowerLeftFront);
    coneMO->normal(lowerLeftFront - lowerRightBack);

    coneMO->position(tip);
    coneMO->normal(tip);

    coneMO->quad(0, 1, 2, 3);
    coneMO->triangle(1, 0, 4);
    coneMO->triangle(2, 1, 4);
    coneMO->triangle(3, 2, 4);
    coneMO->triangle(0, 3, 4);

    coneMO->end();


    Ogre::MeshPtr coneMesh = coneMO->convertToMesh("coneMesh");

    Ogre::Entity* coneEnt = pSceneManager->createEntity(coneMesh);
    m_coneNode = pSceneManager->getRootSceneNode()->createChildSceneNode();
    m_coneNode->attachObject(coneEnt);

    m_coneNode->setVisible(true);



    const Ogre::Vector3 conePos(0, 500, 1500);
    const Ogre::Vector3 coneScale(50.0f);


    m_coneNode->setScale(coneScale);

    //create physical actor
    m_coneActor = physXScene->createRigidDynamic(coneEnt, m_density, coneScale);

//    setupFiltering(fish1Actor.getPxActor(), eFish, eAll);

//    physx::PxVec3 vel1(0, 50, 0);
//    fish1Actor.getPxActor()->setLinearVelocity(vel1);


    //setup binding
    physXScene->createRenderedActorBinding(m_coneActor, new OgrePhysX::NodeRenderable(m_coneNode));

    m_coneActor.setGlobalPosition(conePos);

//    Ogre::Quaternion rot;
//    rot.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_X);

//    m_coneActor.setGlobalOrientation(rot);


    m_polyhedron = MeshTriangleConverter::convert(coneMesh);


}


















