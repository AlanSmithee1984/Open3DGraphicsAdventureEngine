#include "objectmover.h"


ObjectMover::ObjectMover()
{
}

void ObjectMover::addObject(Ogre::InstancedGeometry::InstancedObject *obj)
{
    m_objects << obj;
}

bool ObjectMover::frameRenderingQueued(const Ogre::FrameEvent &evt)
{

    quint32 i = 0;

    foreach(Ogre::InstancedGeometry::InstancedObject* obj, m_objects)
    {
        Ogre::Vector3 pos = obj->getPosition();

        switch (i % 3)
        {

        case 0:
        {
            pos.x += 1;
            break;
        }

        case 1:
        {
            pos.y += 1;
            break;
        }

        case 2:
        {
            pos.z += 1;
            break;
        }


        default:
        {
            Q_ASSERT(false);
            break;
        }

        }

        obj->setPosition(pos);

        ++i;
    }

    return true;
}
