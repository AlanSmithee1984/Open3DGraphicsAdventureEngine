#include "skyxframelistener.h"

#include <SkyX.h>
#include <Hydrax.h>

#include "skyxsettings.h"

SkyXFrameListener::SkyXFrameListener(Ogre::SceneManager* sceneManger,
                                     Ogre::RenderWindow* window,
                                     Ogre::Camera* camera)
    : m_camera(camera),
      mForceDisableShadows(false),
      m_skyX(NULL),
      mBasicController(NULL),
      m_hydrax(NULL),
      m_sunLight(NULL),
      m_shadowLight(NULL),
      mLastPositionLength(0)
{
    // Create SkyX
    mBasicController = new SkyX::BasicController();
    m_skyX = new SkyX::SkyX(sceneManger, mBasicController);

    // A little change to default atmosphere settings :)
    SkyX::AtmosphereManager::Options atOpt = m_skyX->getAtmosphereManager()->getOptions();
    atOpt.RayleighMultiplier = 0.003075f;
    atOpt.MieMultiplier = 0.00125f;
    atOpt.InnerRadius = 9.92f;
    atOpt.OuterRadius = 10.3311f;
    m_skyX->getAtmosphereManager()->setOptions(atOpt);



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
    m_skyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2, -1));

    // Register SkyX listeners
    Ogre::Root::getSingletonPtr()->addFrameListener(m_skyX);

    // Since our two viewports are created through the mWindow render window, we've just add SkyX as a RenderTargetListener
    // and SkyX will automatically handle all the multi-camera stuff.
    // In very specific applications(like editors or when you're using a complex rendering pipeline), you'll need to manually
    // update the SkyX geometry instead of handle it by using listeners. In these situations just invoke SkyX::notifyCameraRender(...)
    // before rendering the camera frame.
    window->addListener(m_skyX);

    Ogre::Root::getSingletonPtr()->addFrameListener(this);


    this->setPreset(mPresets[4]);

    // Light
    m_sunLight = sceneManger->createLight("SunLight");
    m_sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
    m_sunLight->setDiffuseColour(Ogre::ColourValue::White);
    m_sunLight->setSpecularColour(Ogre::ColourValue::White);
    m_sunLight->setCastShadows(false);

    Ogre::Vector3 lightdir(0.0, -1.0, 0.0);
    lightdir.normalise();
    m_sunLight->setDirection(lightdir);




    // Shadow caster
    m_shadowLight = sceneManger->createLight("ShadowLight");
    m_shadowLight->setType(Ogre::Light::LT_DIRECTIONAL);

    m_shadowLight->setDiffuseColour(Ogre::ColourValue::White);
    m_shadowLight->setSpecularColour(Ogre::ColourValue::White);



    this->setColorGradients();

    m_skyX->create();

}

SkyXFrameListener::~SkyXFrameListener()
{

}

void SkyXFrameListener::setHydrax(Hydrax::Hydrax *hydrax)
{
    m_hydrax = hydrax;
}

bool SkyXFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
    // Update environment lighting
    this->updateEnvironmentLighting();

    // Update shadow far distance
    this->updateShadowFarDistance();

    return Ogre::FrameListener::frameStarted(evt);
}

bool SkyXFrameListener::frameEnded(const Ogre::FrameEvent &evt)
{
    return Ogre::FrameListener::frameEnded(evt);
}

SkyX::SkyX *SkyXFrameListener::getSkyX() const
{
    return m_skyX;
}

Ogre::Light *SkyXFrameListener::getSunLight() const
{
    return m_sunLight;
}

void SkyXFrameListener::setPreset(const SkyXSettings& preset)
{
    m_skyX->setTimeMultiplier(preset.timeMultiplier);
    mBasicController->setTime(preset.time);
    mBasicController->setMoonPhase(preset.moonPhase);
    m_skyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

    // Layered clouds
    if (preset.layeredClouds)
    {
        // Create layer cloud
        if (m_skyX->getCloudsManager()->getCloudLayers().empty())
        {
            m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
        }
    }
    else
    {
        // Remove layer cloud
        if (!m_skyX->getCloudsManager()->getCloudLayers().empty())
        {
            m_skyX->getCloudsManager()->removeAll();
        }
    }

    m_skyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
    m_skyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

    SkyX::VClouds::VClouds* vclouds = m_skyX->getVCloudsManager()->getVClouds();

    vclouds->setWindDirection(preset.vcWindDir);
    vclouds->setAmbientColor(preset.vcAmbientColor);
    vclouds->setLightResponse(preset.vcLightResponse);
    vclouds->setAmbientFactors(preset.vcAmbientFactors);
    vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

    if (preset.volumetricClouds)
    {
        // Create VClouds
        if (!m_skyX->getVCloudsManager()->isCreated())
        {
            // SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
            m_skyX->getVCloudsManager()->create(m_skyX->getMeshManager()->getSkydomeRadius(m_camera));
        }
    }
    else
    {
        // Remove VClouds
        if (m_skyX->getVCloudsManager()->isCreated())
        {
            m_skyX->getVCloudsManager()->remove();
        }
    }

    vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
    vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
    vclouds->getLightningManager()->setLightningColor(preset.vcLightningsColor);
    vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);

    m_skyX->update(0);
}

void SkyXFrameListener::setColorGradients()
{
    // Color gradients
    // Water
    mWaterGradient = SkyX::ColorGradient();
    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.779105)*0.4, 1));
    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.729105)*0.3, 0.8));
    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.25, 0.6));
    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.2, 0.5));
    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.1, 0.45));
    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.025, 0));
    // Sun
    mSunGradient = SkyX::ColorGradient();
    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.5, 1.0f));
    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.4, 0.75f));
    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.3, 0.5625f));
    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.6,0.5,0.2)*1.5, 0.5f));
    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.25, 0.45f));
    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.01, 0.0f));
    // Ambient
    mAmbientGradient = SkyX::ColorGradient();
    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 1.0f));
    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 0.6f));
    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.6, 0.5f));
    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.3, 0.45f));
    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.1, 0.35f));
    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.05, 0.0f));
}

void SkyXFrameListener::updateEnvironmentLighting()
{
    SkyX::Controller* controller = m_skyX->getController();
    Ogre::Vector3 lightDir = -controller->getSunDirection();

    std::cout << lightDir << std::endl;

//    bool preForceDisableShadows = mForceDisableShadows;
//    mForceDisableShadows = (lightDir.y > 0.15f) ? true : false;

//    if (preForceDisableShadows != mForceDisableShadows)
//    {
//        setShadowMode(m_sc, static_cast<Ogre::ShadowMode>(mShadowMode));
//    }

    // Calculate current color gradients point
    float point = (-lightDir.y + 1.0f) / 2.0f;
    m_hydrax->setWaterColor(mWaterGradient.getColor(point));

    Ogre::Real skyDomeRadius = m_skyX->getMeshManager()->getSkydomeRadius(m_camera);

    Ogre::Vector3 sunPos = m_camera->getDerivedPosition() - lightDir * skyDomeRadius * 0.1;
    m_hydrax->setSunPosition(sunPos);

    m_sunLight->setPosition(sunPos);
    m_shadowLight->setDirection(lightDir);

    Ogre::Vector3 sunCol = mSunGradient.getColor(point);
    m_sunLight->setSpecularColour(sunCol.x, sunCol.y, sunCol.z);

    Ogre::Vector3 ambientCol = mAmbientGradient.getColor(point);
    m_sunLight->setDiffuseColour(ambientCol.x, ambientCol.y, ambientCol.z);

    m_hydrax->setSunColor(sunCol);
}

void SkyXFrameListener::updateShadowFarDistance()
{
    Ogre::Real currentLength = (Ogre::Vector3(1500, 100, 1500) - m_camera->getDerivedPosition()).length();

    if (currentLength < 1000)
    {
        mLastPositionLength = currentLength;
        return;
    }

    if (currentLength - mLastPositionLength > 100)
    {
        mLastPositionLength += 100;

        m_shadowLight->setShadowFarDistance(m_shadowLight->getShadowFarDistance() + 100);
    }
    else if (currentLength - mLastPositionLength < -100)
    {
        mLastPositionLength -= 100;

        m_shadowLight->setShadowFarDistance(m_shadowLight->getShadowFarDistance() - 100);
    }
}
