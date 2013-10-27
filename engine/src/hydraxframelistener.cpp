#include "hydraxframelistener.h"

#include <Hydrax.h>
#include <Modules/ProjectedGrid/ProjectedGrid.h>

HydraxFrameListener::HydraxFrameListener(Ogre::SceneManager* sceneManger, Ogre::Camera* camera)
{
    this->initHydrax(sceneManger, camera);
}

bool HydraxFrameListener::frameEnded(const Ogre::FrameEvent &evt)
{
    // Update Hydrax
    m_hydrax->update(evt.timeSinceLastFrame);
    return Ogre::FrameListener::frameEnded(evt);
}

void HydraxFrameListener::initHydrax(Ogre::SceneManager* sceneManger, Ogre::Camera* camera)
{
    // Create Hydrax object
    m_hydrax = new Hydrax::Hydrax(sceneManger, camera, camera->getViewport());

    // Create our projected grid module
    Hydrax::Module::ProjectedGrid *mModule
        = new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
                                            m_hydrax,
                                            // Noise module
                                            new Hydrax::Noise::Perlin(/*Generic one*/),
                                            // Base plane
                                            Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
                                            // Normal mode
                                            Hydrax::MaterialManager::NM_VERTEX,
                                            // Projected grid options
                                            Hydrax::Module::ProjectedGrid::Options(/*264*/  /*Generic one*/));

    // Set our module
    m_hydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));

    // Load all parameters from config file
    // Remarks: The config file must be in Hydrax resource group.
    // All parameters can be set/updated directly by code(Like previous versions),
    // but due to the high number of customizable parameters, since 0.4 version, Hydrax allows save/load config files.
    m_hydrax->loadCfg("ocean.hdx");

    // Create water
    m_hydrax->create();
}