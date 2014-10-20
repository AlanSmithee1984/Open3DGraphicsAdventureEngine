#include "buoyancysimulationlistener.h"

#include <floatableobject.h>

BuoyancySimulationListener::BuoyancySimulationListener()
{
}

void BuoyancySimulationListener::addFloatableObject(FloatableObject *obj)
{
    m_objects << obj;
}


void BuoyancySimulationListener::onBeginSimulate(float time)
{
    foreach(FloatableObject* object, m_objects)
    {
        object->updateBuoyancy();
    }
}
