#include "physxsimulationlistener.h"

PhysxSimulationListener::PhysxSimulationListener()
{
}

void PhysxSimulationListener::addChildListener(OgrePhysX::Scene::SimulationListener *listener)
{
    m_childListener << listener;
}

void PhysxSimulationListener::onBeginSimulate(float time)
{
    foreach(OgrePhysX::Scene::SimulationListener* listener, m_childListener)
    {
        listener->onBeginSimulate(time);
    }
}

void PhysxSimulationListener::onSimulate(float time)
{
    foreach(OgrePhysX::Scene::SimulationListener* listener, m_childListener)
    {
        listener->onSimulate(time);
    }
}

void PhysxSimulationListener::onEndSimulate(float time)
{
    foreach(OgrePhysX::Scene::SimulationListener* listener, m_childListener)
    {
        listener->onEndSimulate(time);
    }
}
