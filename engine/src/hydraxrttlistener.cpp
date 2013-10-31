#include "hydraxrttlistener.h"

#include <SkyX.h>

HydraxRttListener::HydraxRttListener(Hydrax::Hydrax* hydrax, SkyX::SkyX* skyX)
    : m_hydrax(hydrax),
      m_skyX(skyX)
{
}


void HydraxRttListener::preRenderTargetUpdate(const Hydrax::RttManager::RttType &Rtt)
{
    // If needed in any case...
    bool underwater = m_hydrax->_isCurrentFrameUnderwater();

    switch (Rtt)
    {

    case Hydrax::RttManager::RTT_REFLECTION:
    {
        // No stars in the reflection map
        m_skyX->setStarfieldEnabled(false);
        break;
    }


    case Hydrax::RttManager::RTT_REFRACTION:
    {
        break;
    }


    case Hydrax::RttManager::RTT_DEPTH:
    case Hydrax::RttManager::RTT_DEPTH_REFLECTION:
    {
        // Hide SkyX components in depth maps
        m_skyX->getMeshManager()->getEntity()->setVisible(false);
        m_skyX->getMoonManager()->getMoonBillboard()->setVisible(false);
        break;
    }

    default:
    {
        // do nothing
        break;
    }



    }
}

void HydraxRttListener::postRenderTargetUpdate(const Hydrax::RttManager::RttType &Rtt)
{
    bool underwater = m_hydrax->_isCurrentFrameUnderwater();

    switch (Rtt)
    {

    case Hydrax::RttManager::RTT_REFLECTION:
    {
        m_skyX->setStarfieldEnabled(true);
        break;
    }


    case Hydrax::RttManager::RTT_REFRACTION:
    {
        break;
    }

    case Hydrax::RttManager::RTT_DEPTH: case Hydrax::RttManager::RTT_DEPTH_REFLECTION:
    {
        m_skyX->getMeshManager()->getEntity()->setVisible(true);
        m_skyX->getMoonManager()->getMoonBillboard()->setVisible(true);
        break;
    }

    default:
    {
        // do nothing
        break;
    }

    }
}
