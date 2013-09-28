#ifndef SCENECREATOR_H
#define SCENECREATOR_H

namespace Ogre
{
class SceneManager;
class Camera;
}

namespace Caelum
{
class CaelumSystem;
}


class SceneCreator
{
public:
    SceneCreator(Ogre::SceneManager* sceneManager, Ogre::Camera *cam);

    void createScene();


private:
    void createHead();
    void createSky();
    Ogre::SceneManager* m_pSceneManager;
    Ogre::Camera* m_pCamera;

    Caelum::CaelumSystem *m_caelumSystem;
};

#endif // SCENECREATOR_H
