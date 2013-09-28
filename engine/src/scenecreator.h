#ifndef SCENECREATOR_H
#define SCENECREATOR_H

namespace Ogre
{
class SceneManager;
class Camera;
class SceneNode;
}

namespace Caelum
{
class CaelumSystem;
}

namespace CCS
{
class CameraControlSystem;
}

class SceneCreator
{
public:
    SceneCreator(Ogre::SceneManager* sceneManager, Ogre::Camera *cam);

    void createScene();


private:
    void createHead();
    void createSky();
    void setupCameraControlSystem();


    Ogre::SceneManager* m_pSceneManager;
    Ogre::Camera* m_pCamera;

    Ogre::SceneNode* m_headNode;

    Caelum::CaelumSystem *m_caelumSystem;
    CCS::CameraControlSystem* m_pCameraCS;


};

#endif // SCENECREATOR_H
