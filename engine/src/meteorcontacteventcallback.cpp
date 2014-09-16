#include "meteorcontacteventcallback.h"

#include <QDebug>

#include <OgreALSound.h>

#include "physx/PxRigidActor.h"

MeteorContactEventCallback::MeteorContactEventCallback()
{
}

void MeteorContactEventCallback::insertActor(const physx::PxActor *shape, OgreAL::Sound *sound)
{
    Q_ASSERT(m_actorToImpactSound.contains(shape) == false);
    m_actorToImpactSound.insert(shape, sound);
}

void MeteorContactEventCallback::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs)
{
    qDebug() << __FUNCTION__;
    //    for(quint32 i = 0 ; i < nbPairs ; ++i)
    //    {
    //        const physx::PxContactPair &pair = pairs[i];
    //        qDebug() << pair.shapes[0] << pair.shapes[1];
    //    }

    const physx::PxActor* actor0 = pairHeader.actors[0];

    if(m_actorToImpactSound.contains(actor0))
    {
        OgreAL::Sound* sound = m_actorToImpactSound.value(actor0);
        sound->play();
    }

    const physx::PxActor* actor1 = pairHeader.actors[1];
    if(m_actorToImpactSound.contains(actor1))
    {
        OgreAL::Sound* sound = m_actorToImpactSound.value(actor1);
        sound->play();
    }
}

void MeteorContactEventCallback::onSleep(physx::PxActor **actors, physx::PxU32 count)
{
    qDebug() << __FUNCTION__;
}

void MeteorContactEventCallback::onWake(physx::PxActor **actors, physx::PxU32 count)
{
    qDebug() << __FUNCTION__;
}

void MeteorContactEventCallback::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
    qDebug() << __FUNCTION__;
}

void MeteorContactEventCallback::onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count)
{
    qDebug() << __FUNCTION__;

    void * owner = constraints->externalReference;

    qDebug() << owner;

//    if()
}
