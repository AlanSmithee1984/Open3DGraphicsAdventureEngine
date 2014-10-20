#ifndef CONE_H
#define CONE_H

#include "floatableobject.h"

namespace Ogre
{
    class SceneManager;
}


class Cone : public FloatableObject
{
public:
    Cone(Ogre::SceneManager *pSceneManager, OgrePhysX::Scene *physXScene);


};

#endif // CONE_H
