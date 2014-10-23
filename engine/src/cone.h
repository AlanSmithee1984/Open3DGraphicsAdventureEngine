#ifndef CONE_H
#define CONE_H

#include "floatableobject.h"

namespace Ogre
{
    class SceneManager;
}


struct OgrePointWithNormal
{
    OgrePointWithNormal()
        : point(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f)
    {

    }

    OgrePointWithNormal(const Ogre::Vector3 &pointTmp, const Ogre::Vector3 &normalTmp)
        : point(pointTmp),
          normal(normalTmp)
    {

    }

    Ogre::Vector3 point;
    Ogre::Vector3 normal;
};


class Cone : public FloatableObject
{
public:
    Cone(Ogre::SceneManager *pSceneManager, OgrePhysX::Scene *physXScene);

private:



    void addNextSlice();


    static std::vector<OgrePointWithNormal> s_templatePoints;


};

#endif // CONE_H
