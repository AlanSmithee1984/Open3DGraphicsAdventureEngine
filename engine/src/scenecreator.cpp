#include "scenecreator.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include <Caelum/Caelum.h>

SceneCreator::SceneCreator(Ogre::SceneManager* sceneManager, Ogre::Camera* cam)
    : m_pSceneManager(sceneManager),
      m_pCamera(cam)
{
}


void SceneCreator::createScene()
{

    this->createHead();
    this->createSky();

}

void SceneCreator::createHead()
{
    // Create an Entity
    Ogre::Entity* ogreHead = m_pSceneManager->createEntity("Head", "ogrehead.mesh");

    // Create a SceneNode and attach the Entity to it
    Ogre::SceneNode* headNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    headNode->attachObject(ogreHead);
}

void SceneCreator::createSky()
{
    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    m_caelumSystem = new Caelum::CaelumSystem (root, m_pSceneManager, Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);

    Ogre::Viewport* viewPort = m_pCamera->getViewport();
    m_caelumSystem->attachViewport(viewPort);

    root->addFrameListener (m_caelumSystem);

//    Caelum::CaelumPlugin::getSingleton().loadCaelumSystemFromScript (m_caelumSystem, "sky_system_name");
}
