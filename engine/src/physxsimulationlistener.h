#ifndef PHYSXSIMULATIONLISTENER_H
#define PHYSXSIMULATIONLISTENER_H

#include <OgrePhysXScene.h>
#include <QList>

class PhysxSimulationListener : public OgrePhysX::Scene::SimulationListener
{
public:
    PhysxSimulationListener();

    void addChildListener(OgrePhysX::Scene::SimulationListener* listener);

    /**
    Called right before PxScene::simulate.
    */
    virtual void onBeginSimulate(float time);

    /**
    Called right after PxScene::simulate.
    That means that it is executed while the simulation is in progress.
    */
    virtual void onSimulate(float time);

    /**
    Called right after PxScene::fetchResults.
    */
    virtual void onEndSimulate(float time);

private:
    QList<OgrePhysX::Scene::SimulationListener*> m_childListener;
};

#endif // PHYSXSIMULATIONLISTENER_H
