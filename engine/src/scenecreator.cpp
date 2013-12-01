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

#include <QtGlobal>

SceneCreator::SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::Camera* cam)
    : m_pSceneManager(sceneManager),
      m_window(window),
      m_pCamera(cam),
      m_headNode(NULL),
      m_fishNode(NULL),
      m_camFrameListener(NULL),
      m_hydraxListener(NULL),
      m_skyXFrameListener(NULL)
{

}

void SceneCreator::createScene()
{
    this->setupCameraControlSystem();
    this->createEnvironment();

    // Hydrax initialization code end -----------------------------------------
    // ------------------------------------------------------------------------

    // Load island
    //		mSceneMgr->setWorldGeometry("Island.cfg");

    Ogre::Light* light = this->createDirectionalLight(); // Make a directional light for terrain
    //        Configure_Terrain(light);


//    Ogre::MaterialPtr islandMat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Island"));


//    std::cout << islandMat->getNumTechniques() << "\t" << islandMat->getNumSupportedTechniques() << std::endl;

    //        mHydrax->getMaterialManager()->addDepthTechnique(islandMat->createTechnique());

//    std::cout << islandMat->getNumTechniques() << "\t" << islandMat->getNumSupportedTechniques() << std::endl;




    // Create palmiers
    //        createPalms(mSceneMgr);

    // Create text area to show skyboxes information
//    createTextArea();



    Ogre::Entity* fish = m_pSceneManager->createEntity("FishEntity", "fish.mesh");

    //        Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName("fish");
    //        Ogre::Pass* pass = materialPtr->getTechnique(0)->getPass(0);
    //        pass->getTextureUnitState(0)->setTextureName("island.tga");

    //        fish->setMaterial(materialPtr);

//    std::cout << fish->getNumSubEntities() << std::endl;

//    Ogre::MaterialPtr fishMat = fish->getSubEntity(0)->getMaterial();

//    std::cout << fishMat->getNumTechniques() << "\t" << fishMat->getNumSupportedTechniques() << std::endl;

//    mHydrax->getMaterialManager()->addDepthTechnique(fishMat->createTechnique());

//    std::cout << fishMat->getNumTechniques() << "\t" << fishMat->getNumSupportedTechniques() << std::endl;

    // Create a SceneNode and attach the Entity to it
    Ogre::SceneNode* m_fishNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("FishNode");
    m_fishNode->attachObject(fish);

    m_fishNode->setPosition(1000, 50, 0);
    m_fishNode->setScale(100, 100, 100);






    // Create a prefab plane
    Ogre::Entity* cubeEnt = m_pSceneManager->createEntity("Cube", Ogre::SceneManager::PT_CUBE);

    // Give the plane a texture
    cubeEnt->setMaterialName("Examples/BumpyMetal");

    Ogre::MaterialPtr boxMat = cubeEnt->getSubEntity(0)->getMaterial();

    std::cout << boxMat->getNumTechniques() << "\t" << boxMat->getNumSupportedTechniques() << std::endl;
    Hydrax::Hydrax* mHydrax = m_hydraxListener->getHydrax();
    mHydrax->getMaterialManager()->addDepthTechnique(boxMat->createTechnique());

    std::cout << boxMat->getNumTechniques() << "\t" << boxMat->getNumSupportedTechniques() << std::endl;


    // Attach the 2 new entities to the root of the scene
    Ogre::SceneNode* cubeNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("CubeNode");
    cubeNode->attachObject(cubeEnt);

    cubeNode->setPosition(1000, 100, 1000);




}



//void SceneCreator::createScene()
//{


//    this->createHead();

//    this->setupCameraControlSystem();

//    this->createEnvironment();


//    this->createFish();

//    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
//    lightdir.normalise();

//    Ogre::Light* light = m_pSceneManager->createLight("tstLight");
//    light->setPosition(0, 300, 2000);
//    light->setType(Ogre::Light::LT_POINT);
//    light->setDirection(lightdir);
//    light->setDiffuseColour(Ogre::ColourValue::White);
//    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

////    this->createTerrain(light);

//    // Add the Hydrax depth technique to island material
////    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();

////    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
////    while(ti.hasMoreElements())
////    {
////        Ogre::Terrain* t = ti.getNext()->instance;
////        Ogre::MaterialPtr ptr = t->getMaterial();
////        hydrax->getMaterialManager()->addDepthTechnique(ptr->createTechnique());
////    }



//}

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

    m_hydraxListener = new HydraxFrameListener(m_pSceneManager, m_pCamera);

    //    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();
    //    m_skyXFrameListener = new SkyXFrameListener(m_pSceneManager, m_window, m_pCamera, hydrax);

    //    // Add the Hydrax Rtt listener
    //    SkyX::SkyX* skyX = m_skyXFrameListener->getSkyX();
    //    HydraxRttListener* rttListener = new HydraxRttListener(hydrax, skyX);
    //    hydrax->getRttManager()->addRttListener(rttListener);


}


Ogre::Light* SceneCreator::createDirectionalLight()
{
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();

    Ogre::Light* light = m_pSceneManager->createLight("tstLight");
    light->setPosition(2000, 300, 500);
    light->setType(Ogre::Light::LT_POINT);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

    m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

//        mHydrax->setSunPosition(light->getPosition());

    return light;
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

void SceneCreator::createTerrain(Ogre::Light* light)
{
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(m_pSceneManager, Ogre::Terrain::ALIGN_X_Z, 129, 3000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("ExileDemoTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    //    Ogre::Light* sunLight = m_skyXFrameListener->getSunLight();



    m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

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

