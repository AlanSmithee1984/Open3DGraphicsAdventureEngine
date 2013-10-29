#include "skyxframelistener.h"

#include <SkyX.h>
#include "skyxsettings.h"

SkyXFrameListener::SkyXFrameListener(Ogre::SceneManager* sceneManger, Ogre::RenderWindow* window, Ogre::Camera* camera)
    : m_camera(camera)
{
    // Create SkyX
    mBasicController = new SkyX::BasicController();
    mSkyX = new SkyX::SkyX(sceneManger, mBasicController);

    // A little change to default atmosphere settings :)
    SkyX::AtmosphereManager::Options atOpt = mSkyX->getAtmosphereManager()->getOptions();
    atOpt.RayleighMultiplier = 0.003075f;
    atOpt.MieMultiplier = 0.00125f;
    atOpt.InnerRadius = 9.92f;
    atOpt.OuterRadius = 10.3311f;
    mSkyX->getAtmosphereManager()->setOptions(atOpt);


    mSkyX->create();

    // Distance geometry falling is a feature introduced in SkyX 0.2
    // When distance falling is enabled, the geometry linearly falls with the distance and the
    // amount of falling in world units is determinated by the distance between the cloud field "plane"
    // and the camera height multiplied by the falling factor.
    // For this demo, a falling factor of two is good enough for the point of view we're using. That means that if the camera
    // is at a distance of 100 world units from the cloud field, the fartest geometry will fall 2*100 = 200 world units.
    // This way the cloud field covers a big part of the sky even if the camera is in at a very low altitude.
    // The second parameter is the max amount of falling distance in world units. That's needed when for example, you've an
    // ocean and you don't want to have the volumetric cloud field geometry falling into the water when the camera is underwater.
    // -1 means that there's not falling limit.
    mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));

    // Register SkyX listeners
    Ogre::Root::getSingletonPtr()->addFrameListener(mSkyX);
    // Since our two viewports are created through the mWindow render window, we've just add SkyX as a RenderTargetListener
    // and SkyX will automatically handle all the multi-camera stuff.
    // In very specific applications(like editors or when you're using a complex rendering pipeline), you'll need to manually
    // update the SkyX geometry instead of handle it by using listeners. In these situations just invoke SkyX::notifyCameraRender(...)
    // before rendering the camera frame.
    window->addListener(mSkyX);

    Ogre::Root::getSingletonPtr()->addFrameListener(this);


    this->setPreset(mPresets[0]);

}

SkyXFrameListener::~SkyXFrameListener()
{

}

bool SkyXFrameListener::frameEnded(const Ogre::FrameEvent &evt)
{
    return Ogre::FrameListener::frameEnded(evt);
}

void SkyXFrameListener::setPreset(const SkyXSettings& preset)
{
    mSkyX->setTimeMultiplier(preset.timeMultiplier);
    mBasicController->setTime(preset.time);
    mBasicController->setMoonPhase(preset.moonPhase);
    mSkyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

    // Layered clouds
    if (preset.layeredClouds)
    {
        // Create layer cloud
        if (mSkyX->getCloudsManager()->getCloudLayers().empty())
        {
            mSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
        }
    }
    else
    {
        // Remove layer cloud
        if (!mSkyX->getCloudsManager()->getCloudLayers().empty())
        {
            mSkyX->getCloudsManager()->removeAll();
        }
    }

    mSkyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
    mSkyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

    SkyX::VClouds::VClouds* vclouds = mSkyX->getVCloudsManager()->getVClouds();

    vclouds->setWindDirection(preset.vcWindDir);
    vclouds->setAmbientColor(preset.vcAmbientColor);
    vclouds->setLightResponse(preset.vcLightResponse);
    vclouds->setAmbientFactors(preset.vcAmbientFactors);
    vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

    if (preset.volumetricClouds)
    {
        // Create VClouds
        if (!mSkyX->getVCloudsManager()->isCreated())
        {
            // SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
            mSkyX->getVCloudsManager()->create(mSkyX->getMeshManager()->getSkydomeRadius(m_camera));
        }
    }
    else
    {
        // Remove VClouds
        if (mSkyX->getVCloudsManager()->isCreated())
        {
            mSkyX->getVCloudsManager()->remove();
        }
    }

    vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
    vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
    vclouds->getLightningManager()->setLightningColor(preset.vcLightningsColor);
    vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);

//    mTextArea->setCaption(buildInfoStr());

    mSkyX->update(0);
}
