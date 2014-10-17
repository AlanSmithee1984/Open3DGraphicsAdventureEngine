#include "hydraxframelistener.h"

#include <Hydrax.h>
#include <Modules/ProjectedGrid/ProjectedGrid.h>

#include <QtGlobal>

#include "oceanheightobserver.h"

HydraxFrameListener::HydraxFrameListener(Ogre::SceneManager* sceneManger, Ogre::Camera* camera)
{
    Ogre::Viewport* viewport = camera->getViewport();
    // Create Hydrax object
    m_hydrax = new Hydrax::Hydrax(sceneManger, camera, viewport);

    Hydrax::Module::ProjectedGrid::Options opts;

    Ogre::Real waterHeight = 0.0f;

    // Create our projected grid module
    Hydrax::Module::ProjectedGrid *mModule
            = new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
                                                m_hydrax,
                                                // Noise module
                                                new Hydrax::Noise::Perlin(/*Generic one*/),
                                                // Base plane
                                                Ogre::Plane(Ogre::Vector3::UNIT_Y, waterHeight),
                                                // Normal mode
                                                Hydrax::MaterialManager::NM_VERTEX,
                                                // Projected grid options
                                                opts);
    // Set our module
    m_hydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));

    // Load all parameters from config file
    // Remarks: The config file must be in Hydrax resource group.
    // All parameters can be set/updated directly by code(Like previous versions),
    // but due to the high number of customizable parameters, since 0.4 version, Hydrax allows save/load config files.
    bool loadSuccess = m_hydrax->loadCfg("ocean.hdx");
    Q_ASSERT(loadSuccess);

    // Create water
    m_hydrax->create();


//    m_hydrax->setVisible(true);



//    Ogre::Vector3 sunPos(0, 500, 500);
//    m_hydrax->setSunPosition(sunPos);

//    Ogre::Vector3 sunCol(1, 0.9, 0.6);
//    m_hydrax->setSunColor(sunCol);

//    m_hydrax->getMaterialManager()->addDepthTechnique(
//        static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Ogre/Skin"))
//        ->createTechnique());

    Ogre::Root::getSingletonPtr()->addFrameListener(this);
}

bool HydraxFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
    // Update Hydrax
    m_hydrax->update(evt.timeSinceLastFrame);

    foreach(OceanHeightObserver* observer, m_oceanHeigthObservers)
    {
        observer->oceanHeightUpdated(m_hydrax);
    }

    bool ret = Ogre::FrameListener::frameStarted(evt);

    return ret;
}

Hydrax::Hydrax *HydraxFrameListener::getHydrax() const
{
    return m_hydrax;
}

void HydraxFrameListener::addHeightObserver(OceanHeightObserver *observer)
{
    m_oceanHeigthObservers << observer;
}

