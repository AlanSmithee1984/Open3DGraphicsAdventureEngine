#ifndef SCENECREATOR_H
#define SCENECREATOR_H

namespace Ogre
{
class SceneManager;
class Camera;
class RenderWindow;
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

class CameraControlSystemFrameListener;

namespace Hydrax
{
class Hydrax;
}

class SceneCreator
{
public:
    SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow *window, Ogre::Camera *cam);

    void createScene();


private:
    void createHead();
    void createSky();
    void setupCameraControlSystem();
    void createWater();

    CameraControlSystemFrameListener* mFrameListener;


    Ogre::SceneManager* m_pSceneManager;
    Ogre::RenderWindow* m_window;
    Ogre::Camera* m_pCamera;

    Ogre::SceneNode* m_headNode;

    Caelum::CaelumSystem *m_caelumSystem;
    CCS::CameraControlSystem* m_pCameraCS;

    Hydrax::Hydrax* m_hydrax;

};

#endif // SCENECREATOR_H
