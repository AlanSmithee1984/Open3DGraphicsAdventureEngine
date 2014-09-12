#ifndef PHYSXDEBUGSIMULATIONLISTENER_H
#define PHYSXDEBUGSIMULATIONLISTENER_H

#include <OgrePhysXScene.h>

class SimpleLineCollection;

class PhysxDebugSimulationListener : public OgrePhysX::Scene::SimulationListener
{
public:
    PhysxDebugSimulationListener(OgrePhysX::Scene* physxScene, SimpleLineCollection* coll);

    virtual void onEndSimulate(float time);

private:
    OgrePhysX::Scene* m_physxScene;
    SimpleLineCollection* m_lineCollection;
};

#endif // PHYSXDEBUGSIMULATIONLISTENER_H
