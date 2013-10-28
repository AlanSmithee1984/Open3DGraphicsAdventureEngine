#include "scenecreator.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include <Caelum/Caelum.h>

#include "cameracontrolsystemframelistener.h"
#include "hydraxframelistener.h"

#include <QtGlobal>

SceneCreator::SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::Camera* cam)
    : m_pSceneManager(sceneManager),
      m_window(window),
      m_pCamera(cam),
      m_headNode(NULL),
      m_caelumSystem(NULL)
{
}


void SceneCreator::createScene()
{

    this->createHead();
//    this->createSky();
    this->createWater();

    this->setupCameraControlSystem();

}

void SceneCreator::createHead()
{
    // Create an Entity
    Ogre::Entity* headEntity = m_pSceneManager->createEntity("Head", "ogrehead.mesh");

    // Create a SceneNode and attach the Entity to it
    m_headNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    m_headNode->attachObject(headEntity);
}

void SceneCreator::createWater()
{
    m_hydraxListener = new HydraxFrameListener(m_pSceneManager, m_pCamera);
}


void SceneCreator::createSky()
{
    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    m_caelumSystem = new Caelum::CaelumSystem (root, m_pSceneManager,
                                               Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);


    Ogre::SceneNode* camNode = m_caelumSystem->getCaelumCameraNode();
    Q_ASSERT(camNode);

    Ogre::SceneNode* groundNode = m_caelumSystem->getCaelumGroundNode();
    Q_ASSERT(groundNode);

    m_caelumSystem->setSkyDome (new Caelum::SkyDome (m_pSceneManager, camNode));

    Caelum::SphereSun* sun = new Caelum::SphereSun(m_pSceneManager, camNode);
//    sun->setAutoDisableThreshold (0.05);
//    sun->setAutoDisable (true);
    sun->setLightColour(Ogre::ColourValue::Red);
//    sun->setDiffuseMultiplier (Ogre::ColourValue (1, 10, 1));
//    sun->setLightDirection(Ogre::Vector3::UNIT_X);
//    sun->forceFarRadius(0.1);
    m_caelumSystem->setSun (sun);

//    // moon stuff
//    Caelum::Moon* moon = new Caelum::Moon(m_pSceneManager, camNode);
//    m_caelumSystem->setMoon(moon);

    // cloud stuff
    Caelum::CloudSystem* cloudSys = new Caelum::CloudSystem (m_pSceneManager, groundNode);
    cloudSys->createLayerAtHeight(300);
    cloudSys->createLayerAtHeight(500);
    cloudSys->getLayer(0)->setCloudSpeed(Ogre::Vector2(0.000005, -0.000009));
    cloudSys->getLayer(1)->setCloudSpeed(Ogre::Vector2(0.0000045, -0.0000085));
    m_caelumSystem->setCloudSystem (cloudSys);

//    // stars stuff
//    Caelum::PointStarfield* stars = new Caelum::PointStarfield (m_pSceneManager, camNode);
//    stars->addRandomStars(1000);
//    m_caelumSystem->setPointStarfield (stars);


//    // ground fog
//    Caelum::GroundFog* fog = new Caelum::GroundFog(m_pSceneManager, camNode);
//    fog->setDensity(0.001);
//    fog->setVerticalDecay(100);
//    m_caelumSystem->setGroundFog(fog);


    Ogre::Viewport* viewPort = m_pCamera->getViewport();
    m_caelumSystem->attachViewport(viewPort);

    root->addFrameListener (m_caelumSystem);
    m_window->addListener(m_caelumSystem);



    m_caelumSystem->setSceneFogDensityMultiplier (0.0015);
    m_caelumSystem->setManageAmbientLight (true);
    m_caelumSystem->setMinimumAmbientLight (Ogre::ColourValue (0.1, 0.1, 0.1));


}

void SceneCreator::setupCameraControlSystem()
{
    m_camFrameListener = new CameraControlSystemFrameListener(m_window, m_pSceneManager, m_pCamera);
//    mFrameListener->showDebugOverlay(true);
    Ogre::Root::getSingletonPtr()->addFrameListener(m_camFrameListener);


}

