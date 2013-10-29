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

class CameraControlSystemFrameListener;
class HydraxFrameListener;
class SkyXFrameListener;

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



    Ogre::SceneManager* m_pSceneManager;
    Ogre::RenderWindow* m_window;
    Ogre::Camera* m_pCamera;

    Ogre::SceneNode* m_headNode;

    CameraControlSystemFrameListener* m_camFrameListener;
    HydraxFrameListener* m_hydraxListener;
    SkyXFrameListener* m_skyXFrameListener;

//    Caelum::CaelumSystem *m_caelumSystem;



};

#endif // SCENECREATOR_H
