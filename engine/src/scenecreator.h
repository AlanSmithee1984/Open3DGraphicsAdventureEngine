#ifndef SCENECREATOR_H
#define SCENECREATOR_H

namespace Ogre
{
class SceneManager;
class Camera;
class RenderWindow;
class SceneNode;
class Terrain;
class Image;
class Light;
class TerrainGlobalOptions;
class TerrainGroup;
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

    void setupCameraControlSystem();

    // New OGRE Terrain system
    void createTerrain();
    void initBlendMaps(Ogre::Terrain* terrain);
    void defineTerrain(long x, long y);
    // should be a static local function
    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
    void configureTerrainDefaults(Ogre::Light* light);
    void destroyScene(void);

    void createEnvironment();


    void createSphere();





    Ogre::SceneManager* m_pSceneManager;
    Ogre::RenderWindow* m_window;
    Ogre::Camera* m_pCamera;

    Ogre::SceneNode* m_headNode;
    Ogre::SceneNode* m_sphereNode;

    CameraControlSystemFrameListener* m_camFrameListener;
    HydraxFrameListener* m_hydraxListener;
    SkyXFrameListener* m_skyXFrameListener;

    Ogre::TerrainGlobalOptions   *mTerrainGlobals;
    Ogre::TerrainGroup         *mTerrainGroup;
    bool mTerrainsImported;
};

#endif // SCENECREATOR_H
