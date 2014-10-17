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
    class ManualObject;
    class Entity;
}

class CameraControlSystemFrameListener;
class HydraxFrameListener;
class SkyXFrameListener;

namespace OgrePhysX
{
    class Scene;
}

namespace OgreAL
{
    class SoundManager;
}

class SceneCreator
{
public:
    SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow *window, Ogre::Camera *cam);


    void createScene();


private:
    void createCone();
    void createHead();

    void setupCameraControlSystem();

    // New OGRE Terrain system
    void createTerrain(const Ogre::Light *light);
    void initBlendMaps(Ogre::Terrain* terrain);
    void defineTerrain(long x, long y);
    // should be a static local function
    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
    void configureTerrainDefaults(const Ogre::Light *light);
    void destroyScene(void);

    void createEnvironment();


    void createFish();


    void createSounds();

    void createPhysics();

    double generateNoise(const double &start, const double &end) const;
    Ogre::Vector3 generateNoiseVector(const double &start, const double &end) const ;


    void createQuad();


    void setupInstancedMaterialToEntity(Ogre::Entity*ent);
    Ogre::String buildInstancedMaterial(const Ogre::String &originalMaterialName);



    Ogre::SceneManager* m_pSceneManager;
    Ogre::RenderWindow* m_window;
    Ogre::Camera* m_pCamera;

    Ogre::SceneNode* m_headNode;
    Ogre::SceneNode* m_fishNode;

    CameraControlSystemFrameListener* m_camFrameListener;
    HydraxFrameListener* m_hydraxListener;
    SkyXFrameListener* m_skyXFrameListener;

    Ogre::TerrainGlobalOptions   *mTerrainGlobals;
    Ogre::TerrainGroup         *mTerrainGroup;
    bool mTerrainsImported;
    Ogre::Light *createDirectionalLight();

    OgrePhysX::Scene *m_physXScene;

    Ogre::ManualObject* m_quad;

    OgreAL::SoundManager* m_soundManager;

};

#endif // SCENECREATOR_H
