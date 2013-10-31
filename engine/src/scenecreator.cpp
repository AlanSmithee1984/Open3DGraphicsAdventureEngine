#include "scenecreator.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>

//#include <Caelum/Caelum.h>


#include "cameracontrolsystemframelistener.h"
#include "hydraxframelistener.h"
#include "skyxframelistener.h"

#include "hydraxrttlistener.h"

#include <QtGlobal>

SceneCreator::SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::Camera* cam)
    : m_pSceneManager(sceneManager),
      m_window(window),
      m_pCamera(cam),
      m_headNode(NULL),
      m_sphereNode(NULL),
      m_camFrameListener(NULL),
      m_hydraxListener(NULL),
      m_skyXFrameListener(NULL)
{
}


void SceneCreator::createScene()
{

    this->createHead();
    this->createSphere();

    this->createEnvironment();

    this->setupCameraControlSystem();

}

void SceneCreator::createHead()
{
    // Create an Entity
    Ogre::Entity* headEntity = m_pSceneManager->createEntity("Head", "ogrehead.mesh");

    // Create a SceneNode and attach the Entity to it
    m_headNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    m_headNode->attachObject(headEntity);

    m_headNode->setPosition(0, 200, 0);
}

void SceneCreator::createSphere()
{
    Ogre::Entity* sphere = m_pSceneManager->createEntity("Sphere", "sphere.mesh");

//    Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName("island");
//    Ogre::Pass* pass = materialPtr->getTechnique(0)->getPass(0);
//    pass->getTextureUnitState(0)->setTextureName("island.tga");
//    pass->getTextureUnitState(0)->setTextureScale(1, 1);
//    pass->getTextureUnitState(0)->setTextureScroll(0.5, 0.0);
//    pass->getTextureUnitState(0)->setTextureFiltering(Ogre::TFO_NONE);

//    sphere->setMaterial(materialPtr);

    // Create a SceneNode and attach the Entity to it
    m_sphereNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("SphereNode");
    m_sphereNode->attachObject(sphere);

    m_sphereNode->setPosition(0, -100, 0);
    m_sphereNode->setScale(10, 1, 10);
}

void SceneCreator::setupCameraControlSystem()
{
    m_camFrameListener = new CameraControlSystemFrameListener(m_window, m_pSceneManager, m_pCamera);
}

void SceneCreator::createEnvironment()
{
    m_hydraxListener = new HydraxFrameListener(m_pSceneManager, m_pCamera);

    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();
    m_skyXFrameListener = new SkyXFrameListener(m_pSceneManager, m_window, m_pCamera, hydrax);

    // Add the Hydrax Rtt listener
    SkyX::SkyX* skyX = m_skyXFrameListener->getSkyX();
    hydrax->getRttManager()->addRttListener(new HydraxRttListener(hydrax, skyX));


    // Add the Hydrax depth technique to island material
    hydrax->getMaterialManager()->addDepthTechnique(
                static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("island"))
                ->createTechnique());
}

