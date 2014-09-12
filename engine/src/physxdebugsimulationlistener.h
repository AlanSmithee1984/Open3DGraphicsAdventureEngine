#ifndef PHYSXDEBUGSIMULATIONLISTENER_H
#define PHYSXDEBUGSIMULATIONLISTENER_H

#include <OgrePhysXScene.h>

class PhysxDebugSimulationListener : public OgrePhysX::Scene::SimulationListener
{
public:
    PhysxDebugSimulationListener(OgrePhysX::Scene* physxScene);

    virtual void onEndSimulate(float time);

private:
    OgrePhysX::Scene* m_physxScene;
};

#endif // PHYSXDEBUGSIMULATIONLISTENER_H
