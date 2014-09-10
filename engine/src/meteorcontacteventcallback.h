#ifndef METEORCONTACTEVENTCALLBACK_H
#define METEORCONTACTEVENTCALLBACK_H

#include "PxSimulationEventCallback.h"

#include <QHash>

namespace OgreAL
{
class Sound;
}

class MeteorContactEventCallback : public physx::PxSimulationEventCallback
{
public:
    MeteorContactEventCallback();

    void insertActor(const physx::PxActor *shape, OgreAL::Sound* sound);

    virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);

    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count);
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count);
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);

private:
    QHash<const physx::PxActor*, OgreAL::Sound*> m_actorToImpactSound;
};

#endif // METEORCONTACTEVENTCALLBACK_H
