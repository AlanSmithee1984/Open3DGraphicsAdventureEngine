#ifndef OBJECTMOVER_H
#define OBJECTMOVER_H

#include <OgreFrameListener.h>

#include <OgreInstancedGeometry.h>

#include <QList>

class ObjectMover : public Ogre::FrameListener
{
public:
    ObjectMover();

    void addObject(Ogre::InstancedGeometry::InstancedObject* obj);

protected:
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

private:
    QList<Ogre::InstancedGeometry::InstancedObject*> m_objects;
};

#endif // OBJECTMOVER_H
