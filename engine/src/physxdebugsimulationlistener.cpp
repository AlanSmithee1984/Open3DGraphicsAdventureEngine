#include "physxdebugsimulationlistener.h"

#include <QDebug>

#include "simplelinecollection.h"

#include "physxhelper.h"

PhysxDebugSimulationListener::PhysxDebugSimulationListener(OgrePhysX::Scene* physxScene, SimpleLineCollection *coll)
    : m_physxScene(physxScene),
      m_lineCollection(coll)
{
}

void PhysxDebugSimulationListener::onEndSimulate(float time)
{
    Q_ASSERT(m_physxScene);

    const physx::PxRenderBuffer& rb = m_physxScene->getPxScene()->getRenderBuffer();

//    qDebug() << rb.getNbLines();

    QList<SimpleLine::LineAttributes> attributes;

    for(physx::PxU32 i=0; i < rb.getNbLines(); i++)
    {
            const physx::PxDebugLine& line = rb.getLines()[i];
            // render the line

            Ogre::Vector3 start = PhysxHelper::convertToOgreVector(line.pos0);
            Ogre::Vector3 end = PhysxHelper::convertToOgreVector(line.pos1);
            Ogre::ColourValue startCol = PhysxHelper::convertToOgreColour(line.color0);
            Ogre::ColourValue endCol = PhysxHelper::convertToOgreColour(line.color1);


            attributes << SimpleLine::LineAttributes(start, end, startCol, endCol);
    }

    Q_ASSERT(m_lineCollection);
    m_lineCollection->setLineData(attributes);
}
