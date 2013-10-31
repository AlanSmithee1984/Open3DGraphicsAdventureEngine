#ifndef HYDRAXRTTLISTENER_H
#define HYDRAXRTTLISTENER_H

#include <Hydrax.h>

namespace SkyX
{
class SkyX;
}

class HydraxRttListener : public Hydrax::RttManager::RttListener
{
public:
    HydraxRttListener(Hydrax::Hydrax* hydrax, SkyX::SkyX* skyX);

    void preRenderTargetUpdate(const Hydrax::RttManager::RttType& Rtt);

    void postRenderTargetUpdate(const Hydrax::RttManager::RttType& Rtt);

private:
    Hydrax::Hydrax* m_hydrax;
    SkyX::SkyX* m_skyX;
};

#endif // HYDRAXRTTLISTENER_H
