#include <OgrePhysX.h>

#include "scenecreator.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreRenderWindow.h>

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "cameracontrolsystemframelistener.h"
#include "hydraxframelistener.h"
#include "skyxframelistener.h"

#include "hydraxrttlistener.h"

#include <Hydrax/Modules/ProjectedGrid/ProjectedGrid.h>

//#include <NxOgre.h>

#include <OgreALSoundManager.h>



#include <QtGlobal>

#include <QDebug>

SceneCreator::SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::Camera* cam)
    : m_pSceneManager(sceneManager),
      m_window(window),
      m_pCamera(cam),
      m_headNode(NULL),
      m_fishNode(NULL),
      m_camFrameListener(NULL),
      m_hydraxListener(NULL),
      m_skyXFrameListener(NULL),
      m_quad(NULL)
{
//    soundManager = new OgreAL::SoundManager();    OgreAL::SoundManager::getSingletonPtr()->


}



void SceneCreator::createScene()
{


    this->createHead();

    this->setupCameraControlSystem();

    this->createEnvironment();


    this->createFish();


    Ogre::Light* sunLight = m_skyXFrameListener->getSunLight();

    this->createTerrain(sunLight);

//     Add the Hydrax depth technique to island material
//    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();

//    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
//    while(ti.hasMoreElements())
//    {
//        Ogre::Terrain* t = ti.getNext()->instance;
//        Ogre::MaterialPtr ptr = t->getMaterial();
//        hydrax->getMaterialManager()->addDepthTechnique(ptr->createTechnique());
//    }


//    this->createSounds();

    this->createPhysics();


    this->createQuad();

}

void SceneCreator::createHead()
{
    // Create an Entity
    Ogre::Entity* headEntity = m_pSceneManager->createEntity("Head", "ogrehead.mesh");

    // Create a SceneNode and attach the Entity to it
    m_headNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    m_headNode->attachObject(headEntity);

    m_headNode->setPosition(0, 500, 0);
}

void SceneCreator::setupCameraControlSystem()
{
    m_camFrameListener = new CameraControlSystemFrameListener(m_window, m_pSceneManager, m_pCamera);
}

void SceneCreator::createEnvironment()
{

    Q_ASSERT(m_window->getViewport(0) == m_pCamera->getViewport());

    m_skyXFrameListener = new SkyXFrameListener(m_pSceneManager, m_window, m_pCamera);

//    m_hydraxListener = new HydraxFrameListener(m_pSceneManager, m_pCamera);
//    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();
//    m_skyXFrameListener->setHydrax(hydrax);


    // Add the Hydrax Rtt listener
//    SkyX::SkyX* skyX = m_skyXFrameListener->getSkyX();
//    HydraxRttListener* rttListener = new HydraxRttListener(hydrax, skyX);
//    hydrax->getRttManager()->addRttListener(rttListener);


}

void SceneCreator::createFish()
{
    Ogre::MaterialPtr fishMat = Ogre::MaterialManager::getSingletonPtr()->getByName("Examples/Fish");
    Q_ASSERT(fishMat.isNull() == false);



    Ogre::Entity* fishEntity = m_pSceneManager->createEntity("Fish", "fish.mesh");



    //    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();
    //    hydrax->getMaterialManager()->addDepthTechnique(fishMat->createTechnique());

    // Create a SceneNode and attach the Entity to it
    m_fishNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("FishNode");
    m_fishNode->attachObject(fishEntity);

    m_fishNode->setPosition(2000, 50, 0);
    m_fishNode->setScale(100, 100, 100);



}

void SceneCreator::createSounds()
{
    OgreAL::SoundManager* soundManager = OgreAL::SoundManager::getSingletonPtr();
    if(soundManager == NULL)
    {
        soundManager = new OgreAL::SoundManager;
    }

    Ogre::SceneNode* camSceneNode = m_pCamera->getParentSceneNode();

    OgreAL::Listener* soundManagerListener = soundManager->getListener();
    Q_ASSERT(soundManagerListener);
    camSceneNode->attachObject(soundManagerListener);

    OgreAL::Sound *sound = soundManager->createSound("Roar", "roar.wav", true);
    m_headNode->attachObject(sound);
    sound->play();

    OgreAL::Sound *bgSound = soundManager->createSound("ZeroFactor", "Zero Factor - Untitled.ogg", true, true);
    bgSound->setGain(0.5);
    bgSound->setRelativeToListener(true);

    //    OgreAL::SoundManager::getSingletonPtr()->getSound("Roar")->play();
}

void SceneCreator::createPhysics()
{
    OgrePhysX::World::getSingleton().init();

    OgrePhysX::World::getSingleton().setupOgreFramelistener();

    m_physXScene = OgrePhysX::World::getSingleton().addScene("Main", m_pSceneManager);

    //PhyX plane geometry always has the normal (1, 0, 0), so we have to rotate the plane shape in order to create a plane with a normal (0, 1, 0)
    OgrePhysX::PxPlaneGeometry geom = OgrePhysX::Geometry::planeGeometry();

    physx::PxQuat quat(-Ogre::Math::PI/2,
                       physx::PxVec3(0, 0, 1) );
    physx::PxTransform transformation(quat);

    OgrePhysX::Actor<physx::PxRigidStatic> ground = m_physXScene->createRigidStatic(geom, transformation );

    //ground wraps the underlying PxRigidStatic and provides some helper methods
    ground.setGlobalPosition(Ogre::Vector3(0, 0, 0));


    const Ogre::Vector3 globalScale(1000);

    const Ogre::Vector3 debrisPos(0, 0, 0);

    Ogre::Real noiseFaktor = 0;

    //let's do some cool stuff
    OgrePhysX::Destructible *destructible6 = m_physXScene->createDestructible("meteor.xml", 85, 85, 60,
                                                                              Ogre::Vector3(2.0f, 2.0f, 2.0f) * globalScale);
    destructible6->setGlobalPosition(debrisPos + this->generateNoise(0, noiseFaktor));

//    OgrePhysX::Destructible *destructible2 = m_physXScene->createDestructible("meteor.xml", 60, 60, 60,
//                                                                              Ogre::Vector3(1.5f, 1.5f, 1.5f) * globalScale);
//    destructible2->setGlobalPosition(debrisPos + this->generateNoise(0, noiseFaktor));

//    OgrePhysX::Destructible *destructible1 = m_physXScene->createDestructible("meteor.xml", 100, 100, 60,
//                                                                              Ogre::Vector3(1.1f, 1.1f, 1.1f) * globalScale);
//    destructible1->setGlobalPosition(debrisPos + this->generateNoise(0, noiseFaktor));

//    OgrePhysX::Destructible *destructible3 = m_physXScene->createDestructible("meteor.xml", 100, 100, 80,
//                                                                              Ogre::Vector3(1.0f, 1.0f, 1.0f) * globalScale);
//    destructible3->setGlobalPosition(debrisPos + this->generateNoise(0, noiseFaktor));

//    OgrePhysX::Destructible *destructible4 = m_physXScene->createDestructible("meteor.xml", 100, 100, 80,
//                                                                              Ogre::Vector3(1.0f, 1.0f, 1.0f) * globalScale);
//    destructible4->setGlobalPosition(debrisPos + this->generateNoise(0, noiseFaktor));



    Ogre::Entity *ent = m_pSceneManager->createEntity("Kappes", "ogrehead.mesh");
    Ogre::SceneNode *node = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
    node->setVisible(true);
    node->setScale(Ogre::Vector3(10));

    //create physical actor
//    OgrePhysX::Actor<physx::PxRigidDynamic> actor = m_physXScene->createRigidDynamic(ent, 50,
//                                                                                     Ogre::Vector3(1,1,1));

//    //setup binding
//    m_physXScene->createRenderedActorBinding(actor, new OgrePhysX::NodeRenderable(node));

//    actor.setGlobalPosition(Ogre::Vector3(0, 1000, 10));
}

double SceneCreator::generateNoise(const double &start, const double &end) const
{
    double zeroToOne = (double)qrand() / (double)RAND_MAX;

    double range = qAbs(end - start);

    double spreadedValue = zeroToOne * range;

    return spreadedValue + start;
}

void SceneCreator::createQuad()
{

    m_quad = m_pSceneManager->createManualObject();

    Ogre::Vector3 normal = Ogre::Vector3::UNIT_Z;

    m_quad->begin("stonewall");

    m_quad->position(0.0, 0.0, 0.0);
    m_quad->normal(normal);
    m_quad->textureCoord(0.0, 1.0);
//    m_quad->textureCoord(0.0, 1.0);

    m_quad->position(1.0, 0.0, 0.0);
    m_quad->normal(normal);
    m_quad->textureCoord(1.0, 1.0);
//    m_quad->textureCoord(1.0, 1.0);

    m_quad->position(1.0, 1.0, 0.0);
    m_quad->normal(normal);
    m_quad->textureCoord(1.0, 0.0);
//    m_quad->textureCoord(1.0, 0.0);

    m_quad->position(0.0, 1.0, 0.0);
    m_quad->normal(normal);
    m_quad->textureCoord(0.0, 0.0);
//    m_quad->textureCoord(0.0, 0.0);

    m_quad->quad(0, 1, 2, 3);

    m_quad->end();

    Ogre::SceneNode* child = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
    child->attachObject(m_quad);

    child->setScale(Ogre::Vector3(100));
    child->setPosition(50.0, 500, 0);
//    child->showBoundingBox(true);
}

void SceneCreator::createTerrain(Ogre::Light* light)
{
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(m_pSceneManager, Ogre::Terrain::ALIGN_X_Z, 129, 3000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("ExileDemoTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    //    Ogre::Light* sunLight = m_skyXFrameListener->getSunLight();


//    m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    this->configureTerrainDefaults(light);



    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);
    if (mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    mTerrainGroup->freeTemporaryResources();
}

void SceneCreator::initBlendMaps(Ogre::Terrain *terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();

}

void SceneCreator::defineTerrain(long x, long y)
{
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void SceneCreator::getTerrainImage(bool flipX, bool flipY, Ogre::Image &img)
{
    img.load("Island.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)   img.flipAroundY();
    if (flipY)   img.flipAroundX();
}

void SceneCreator::configureTerrainDefaults(Ogre::Light *light)
{
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);       // testing composite map

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(m_pSceneManager->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 129; // 513;
    defaultimp.worldSize = 3000.0f;      // Set up for island.cfg
    defaultimp.inputScale = 200; //600; //
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void SceneCreator::destroyScene()
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

