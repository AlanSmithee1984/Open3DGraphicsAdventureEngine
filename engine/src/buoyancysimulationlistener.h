#ifndef BUOYANCYSIMULATIONLISTENER_H
#define BUOYANCYSIMULATIONLISTENER_H

#include <OgrePhysXScene.h>
#include <QList>

class FloatableObject;

class BuoyancySimulationListener : public OgrePhysX::Scene::SimulationListener
{
public:
    BuoyancySimulationListener();

    void addFloatableObject(FloatableObject* obj);

    virtual void onBeginSimulate(float time);

private:
    QList<FloatableObject*> m_objects;
};



#endif // BUOYANCYSIMULATIONLISTENER_H
