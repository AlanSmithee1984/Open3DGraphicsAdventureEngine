#include "physxdebugsimulationlistener.h"

#include <QDebug>

PhysxDebugSimulationListener::PhysxDebugSimulationListener(OgrePhysX::Scene* physxScene)
    : m_physxScene(physxScene)
{
}

void PhysxDebugSimulationListener::onEndSimulate(float time)
{
    Q_ASSERT(m_physxScene);

    const physx::PxRenderBuffer& rb = m_physxScene->getPxScene()->getRenderBuffer();

    for(physx::PxU32 i=0; i < rb.getNbLines(); i++)
    {
            const physx::PxDebugLine& line = rb.getLines()[i];
            // render the line

//            line.pos1
    }
}
