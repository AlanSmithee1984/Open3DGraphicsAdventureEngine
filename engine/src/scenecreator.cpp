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


#include "meteorcontacteventcallback.h"
#include "physxdebugsimulationlistener.h"
#include "simplelinecollection.h"
#include "objectmover.h"

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
      m_quad(NULL),
      m_soundManager(NULL)
{

}



void SceneCreator::createScene()
{


    this->createHead();

    this->setupCameraControlSystem();

    this->createEnvironment();

    Ogre::Light* sunLight = m_skyXFrameListener->getSunLight();
    this->createTerrain(sunLight);



    this->createFish();


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

    m_hydraxListener = new HydraxFrameListener(m_pSceneManager, m_pCamera);
    Hydrax::Hydrax* hydrax = m_hydraxListener->getHydrax();
    m_skyXFrameListener->setHydrax(hydrax);


    // Add the Hydrax Rtt listener
    SkyX::SkyX* skyX = m_skyXFrameListener->getSkyX();
    HydraxRttListener* rttListener = new HydraxRttListener(hydrax, skyX);
    hydrax->getRttManager()->addRttListener(rttListener);


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
    m_soundManager = OgreAL::SoundManager::getSingletonPtr();
    if(m_soundManager == NULL)
    {
        m_soundManager = new OgreAL::SoundManager;
    }

    Ogre::SceneNode* camSceneNode = m_pCamera->getParentSceneNode();

    OgreAL::Listener* soundManagerListener = m_soundManager->getListener();
    Q_ASSERT(soundManagerListener);
    camSceneNode->attachObject(soundManagerListener);

    //    OgreAL::Sound *sound = soundManager->createSound("Roar", "roar.wav", true);
    //    m_headNode->attachObject(sound);
    //    sound->play();



    //    OgreAL::Sound *bgSound = soundManager->createSound("ZeroFactor", "Zero Factor - Untitled.ogg", true, true);
    //    bgSound->setGain(0.5);
    //    bgSound->setRelativeToListener(true);

    //    OgreAL::SoundManager::getSingletonPtr()->getSound("Roar")->play();
}


physx::PxFilterFlags SampleFilterShader(
        physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
        physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
    // let triggers through
    if(physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
        return physx::PxFilterFlag::eDEFAULT;
    }
    // generate contacts for all that were not filtered above
    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

    // trigger the contact callback for pairs (A,B) where
    // the filtermask of A contains the ID of B and vice versa.
    if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
    {
        pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
    }

    return physx::PxFilterFlag::eDEFAULT;
}

enum FilterGroup
{
    eHEIGHTFIELD    = (1 << 0),
    eMeteor         = (1 << 1),
    eFish           = (1 << 2),
    eAll            = 0xFFFFFFFF
};


void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask)
{
    physx::PxFilterData filterData;
    filterData.word0 = filterGroup; // word0 = own ID
    filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a contact callback;
    const physx::PxU32 numShapes = actor->getNbShapes();

    //    physx::PxShape** shapes = (physx::PxShape**)SAMPLE_ALLOC(sizeof(physx::PxShape*)*numShapes);

    physx::PxShape** shapes = new physx::PxShape*[numShapes];


    actor->getShapes(shapes, numShapes);
    for(physx::PxU32 i = 0; i < numShapes; i++)
    {
        physx::PxShape* shape = shapes[i];
        shape->setSimulationFilterData(filterData);
    }

    delete[] shapes;
}


void SceneCreator::createPhysics()
{
    OgrePhysX::World::getSingleton().init();

    OgrePhysX::World::getSingleton().setupOgreFramelistener();

    MeteorContactEventCallback* callback = new MeteorContactEventCallback;

    physx::PxSceneDesc desc(OgrePhysX::World::getSingleton().getPxPhysics()->getTolerancesScale());
    desc.gravity = physx::PxVec3(0, -9.81f, 0);
    desc.simulationEventCallback = callback;
    //    desc.filterShader = &physx::PxDefaultSimulationFilterShader;
    desc.filterShader = SampleFilterShader;


    m_physXScene = OgrePhysX::World::getSingleton().addScene("Main", m_pSceneManager, desc);


    physx::PxScene* scene = m_physXScene->getPxScene();

//    scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 10.0f);
//    scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_AXES, 2.0f);
//    scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_MASS_AXES, 2.0f);

    SimpleLineCollection* lineColl = new SimpleLineCollection(m_pSceneManager);
    PhysxDebugSimulationListener* simListener = new PhysxDebugSimulationListener(m_physXScene, lineColl);
    m_physXScene->setSimulationListener(simListener);



    //    //PhyX plane geometry always has the normal (1, 0, 0), so we have to rotate the plane shape in order to create a plane with a normal (0, 1, 0)
    //    OgrePhysX::PxPlaneGeometry geom = OgrePhysX::Geometry::planeGeometry();

    //    physx::PxQuat quat(Ogre::Math::PI/2,
    //                       physx::PxVec3(0, 0, 1) );
    //    physx::PxTransform transformation(quat);

    //    OgrePhysX::Actor<physx::PxRigidStatic> ground = m_physXScene->createRigidStatic(geom, transformation );

    //    //ground wraps the underlying PxRigidStatic and provides some helper methods
    //    ground.setGlobalPosition(Ogre::Vector3(0, 200, 0));




    Ogre::Vector3 groundPos(0.0f);



    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* terrain = ti.getNext()->instance;

        Ogre::uint32 numCols, numRows;
        numRows = numCols = terrain->getSize();

        physx::PxHeightFieldDesc heightFieldDesc;
        heightFieldDesc.format             = physx::PxHeightFieldFormat::eS16_TM;
        heightFieldDesc.nbColumns          = numCols;
        heightFieldDesc.nbRows             = numRows;
        heightFieldDesc.samples.stride     = sizeof(physx::PxU32);

        //physx::PxReal heightScale = 1.f;
        Ogre::Real maxHeight = terrain->getMaxHeight() ;
        physx::PxReal heightScale = maxHeight / static_cast<Ogre::Real>(std::numeric_limits<physx::PxI16>::max());
        //        physx::PxReal heightScale = 0.0061035156;


        Ogre::LogManager::getSingletonPtr()->logMessage("*** terrain heigth scale: " + Ogre::StringConverter::toString(heightScale) + " ***");

        physx::PxHeightFieldSample* samples = (physx::PxHeightFieldSample*) new physx::PxHeightFieldSample[numRows*numCols];

        physx::PxU8* currentByte = (physx::PxU8*)samples;

        for (Ogre::uint16  row = 0; row < numRows; row++)
        {
            for (Ogre::uint16 column = 0; column < numCols;  column++)
            {
                physx::PxHeightFieldSample* currentSample = (physx::PxHeightFieldSample*)currentByte;

                Ogre::Real height = *terrain->getHeightData(row, numCols-1-column);

                Ogre::Real heightVal = height / heightScale;

                Q_ASSERT(heightVal <= std::numeric_limits<physx::PxI16>::max());
                Q_ASSERT(heightVal >= std::numeric_limits<physx::PxI16>::min());

                currentSample->height = (physx::PxI16)heightVal;
                currentSample->materialIndex0 = 0;
                currentSample->materialIndex1 = 0;

                //                qDebug() << row << column << currentSample->height;

                currentByte += heightFieldDesc.samples.stride;
            }
        }

        heightFieldDesc.samples.data = samples;

        physx::PxPhysics *physics = &m_physXScene->getPxScene()->getPhysics();
        Q_ASSERT(physics);

        physx::PxHeightField* heightField = physics->createHeightField(heightFieldDesc);

        physx::PxTransform pose = physx::PxTransform::createIdentity();

        /*
                  * Calcul position
                  */
        Ogre::Vector3 position = terrain->getPosition();
        position.x = position.x - (terrain->getWorldSize() / 2.0f);
        //position.y = position.y + ((terrain->getMaxHeight() + terrain->getMinHeight()) / 2.0f);
        position.z = position.z - (terrain->getWorldSize() / 2.0f);

        //row scale
        Ogre::Real scale = terrain->getWorldSize() / physx::PxReal (terrain->getSize() - 1);

        pose.p = physx::PxVec3(position.x , position.y, position.z);
        physx::PxRigidStatic* hfActor = physics->createRigidStatic(pose);


        physx::PxHeightFieldGeometry hfGeom(heightField, physx::PxMeshGeometryFlags(), heightScale, (physx::PxReal) scale, (physx::PxReal) scale);
        physx::PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution
        physx::PxShape* shape = hfActor->createShape(hfGeom, *material);

        physx::PxFilterData collFilterData;
        collFilterData.word0=1;
        collFilterData.word1=1;
        shape->setSimulationFilterData(collFilterData);

        m_physXScene->getPxScene()->addActor(*hfActor);



    }


    //    quint32 terrSize = mTerrainGroup->getDefaultImportSettings().terrainSize;

    //    qreal heightScale = 1.0f;
    //    qreal rowScale = 1.0f;
    //    qreal colScale = 1.0f;

    //    physx::PxHeightFieldDesc desc;
    //    desc.nbColumns = terrSize;
    //    desc.nbRows = terrSize;
    //    desc.thickness = 1.0f;

    //    //    Ogre::Image img;
    //    //    img.load("Island.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    //    char arr[terrSize * terrSize];
    //    memset(arr, 0, terrSize * terrSize);


    //    physx::PxStridedData data;
    //    data.stride = 0;
    ////    data.data = img.getData();
    //    data.data = arr;

    //    desc.samples = data;

    //    OgrePhysX::Actor<physx::PxRigidStatic> groundPlane = m_physXScene->createHeightField(groundPos, terrSize, terrSize, heightScale, rowScale, colScale, desc );
    //    groundPlane.setGlobalPosition(groundPos);





    const Ogre::Vector3 globalScale(10.0f);

    const Ogre::Vector3 debrisPos(0, 1500, 0);



    //let's do some cool stuff
    OgrePhysX::Destructible *centeredMeteor = m_physXScene->createInstanceDestructible("meteor.xml", 85, 85, 60,
                                                                               Ogre::Vector3(2.0f, 2.0f, 2.0f) * globalScale);
    centeredMeteor->setGlobalPosition(debrisPos);



    //    centeredMeteor->setSimulationFilterData();


    const quint32 maxMeteors = 5;
    const Ogre::Real minNoiseFaktor = -250;
    const Ogre::Real maxNoiseFaktor = 250;

    for(quint32 i = 0 ; i < maxMeteors; ++i)
    {
        OgrePhysX::Destructible *destructible2 = m_physXScene->createInstanceDestructible("meteor.xml", 60, 60, 60,
                                                                                  globalScale);
        destructible2->setGlobalPosition(debrisPos + this->generateNoiseVector(minNoiseFaktor, maxNoiseFaktor));
    }



    //    OgrePhysX::Destructible *offsiteMeteor = m_physXScene->createDestructible("meteor.xml", 85, 85, 60,
    //                                                                              Ogre::Vector3(2.0f, 2.0f, 2.0f) * globalScale);
    //    offsiteMeteor->setGlobalPosition(Ogre::Vector3(1000, 500, 0));



    const Ogre::Vector3 fishScale(5.0);



    Ogre::Entity *fish1 = m_pSceneManager->createEntity("PhysicalFish1", "fish.mesh");
    Ogre::SceneNode *fish1Node = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
    fish1Node->attachObject(fish1);
    fish1Node->setVisible(true);
    fish1Node->showBoundingBox(true);






    Ogre::Vector3 fish1Pos(0, 1000, 0);

    fish1Node->setScale(fishScale);

    //create physical actor
    OgrePhysX::Actor<physx::PxRigidDynamic> fish1Actor = m_physXScene->createRigidDynamic(fish1, 100,
                                                                                          fishScale);

    if(m_soundManager)
    {
        OgreAL::Sound* explosion1 = m_soundManager->createSound("Grenade1", "Grenade.wav");
        fish1Node->attachObject(explosion1);
        callback->insertActor(fish1Actor.getPxActor(), explosion1);
    }

    setupFiltering(fish1Actor.getPxActor(), eFish, eAll);

    physx::PxVec3 vel1(0, 50, 0);
    fish1Actor.getPxActor()->setLinearVelocity(vel1);

    physx::PxVec3 force(1000, 50, 100000);
    fish1Actor.getPxActor()->addForce(force, physx::PxForceMode::eIMPULSE);

    //setup binding
    m_physXScene->createRenderedActorBinding(fish1Actor, new OgrePhysX::NodeRenderable(fish1Node));

    fish1Actor.setGlobalPosition(fish1Pos);




    Ogre::Entity *fish2 = m_pSceneManager->createEntity("PhysicalFish2", "fish.mesh");
    Ogre::SceneNode *fish2Node = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
    fish2Node->attachObject(fish2);
    fish2Node->setVisible(true);
    fish2Node->showBoundingBox(true);




    const Ogre::Vector3 fish2Pos(-1000, 1000, 0);

    fish2Node->setScale(fishScale);

    //create physical actor
    OgrePhysX::Actor<physx::PxRigidDynamic> fish2Actor = m_physXScene->createRigidDynamic(fish2, 100,
                                                                                          fishScale);


    if(m_soundManager)
    {
        OgreAL::Sound* explosion2 = m_soundManager->createSound("Grenade2", "Grenade.wav");
        fish2Node->attachObject(explosion2);
        callback->insertActor(fish2Actor.getPxActor(), explosion2);
    }

    setupFiltering(fish2Actor.getPxActor(), eFish, eAll);

    physx::PxVec3 vel2(100, 50, 0);
    fish2Actor.getPxActor()->setLinearVelocity(vel2);




    //setup binding
    m_physXScene->createRenderedActorBinding(fish2Actor, new OgrePhysX::NodeRenderable(fish2Node));

    fish2Actor.setGlobalPosition(fish2Pos);

}

double SceneCreator::generateNoise(const double &start, const double &end) const
{
    double zeroToOne = (double)qrand() / (double)RAND_MAX;

    double range = qAbs(end - start);

    double spreadedValue = zeroToOne * range;

    return spreadedValue + start;

}

Ogre::Vector3 SceneCreator::generateNoiseVector(const double &start, const double &end) const
{
    double x = this->generateNoise(start, end);
    double y = this->generateNoise(start, end);
    double z = this->generateNoise(start, end);

    return Ogre::Vector3(x, y, z);
}

void SceneCreator::createQuad()
{

    m_quad = m_pSceneManager->createManualObject();

    Ogre::Vector3 normal = Ogre::Vector3::UNIT_Z;
    Ogre::Vector3 tangent = Ogre::Vector3::UNIT_X;


    Ogre::String matName = "stonewall";

    m_quad->begin(matName);

    m_quad->position(0.0, 0.0, 0.0);
    m_quad->normal(normal);
    m_quad->tangent(tangent);
    m_quad->textureCoord(0.0, 1.0);
    //    m_quad->textureCoord(0.0, 1.0);

    m_quad->position(1.0, 0.0, 0.0);
    m_quad->normal(normal);
    m_quad->tangent(tangent);
    m_quad->textureCoord(1.0, 1.0);
    //    m_quad->textureCoord(1.0, 1.0);

    m_quad->position(1.0, 1.0, 0.0);
    m_quad->normal(normal);
    m_quad->tangent(tangent);
    m_quad->textureCoord(1.0, 0.0);
    //    m_quad->textureCoord(1.0, 0.0);

    m_quad->position(0.0, 1.0, 0.0);
    m_quad->normal(normal);
    m_quad->tangent(tangent);
    m_quad->textureCoord(0.0, 0.0);
    //    m_quad->textureCoord(0.0, 0.0);

    m_quad->quad(0, 1, 2, 3);

    m_quad->end();

    Ogre::SceneNode* node = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
    node->attachObject(m_quad);



}

void SceneCreator::createTerrain(const Ogre::Light* light)
{
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(m_pSceneManager, Ogre::Terrain::ALIGN_X_Z, 129, 3000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("ExileDemoTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    //    Ogre::Light* sunLight = m_skyXFrameListener->getSunLight();


//    m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    this->configureTerrainDefaults(light);



    for (long x = 0; x <= 0; ++x)
    {
        for (long y = 0; y <= 0; ++y)
        {
            defineTerrain(x, y);
        }
    }

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
        this->getTerrainImage(x % 2 != 0, y % 2 != 0, img);

        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void SceneCreator::getTerrainImage(bool flipX, bool flipY, Ogre::Image &img)
{
    img.load("Island.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
    {
        img.flipAroundY();
    }

    if (flipY)
    {
        img.flipAroundX();
    }
}

void SceneCreator::configureTerrainDefaults(const Ogre::Light *light)
{
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);       // testing composite map

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data

    const Ogre::Vector3 &lightDir = light->getDerivedDirection();
    std::cout << "terr lightdir: " << lightDir << std::endl;

    mTerrainGlobals->setLightMapDirection(lightDir);
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

