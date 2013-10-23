#include "scenecreator.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include <Caelum/Caelum.h>

#include <CCSCameraControlSystem.h>
#include <CCSBasicCameraModes.h>

#include "cameracontrolsystemframelistener.h"

SceneCreator::SceneCreator(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::Camera* cam)
    : m_pSceneManager(sceneManager),
      m_window(window),
      m_pCamera(cam),
      m_headNode(NULL),
      m_caelumSystem(NULL),
      m_pCameraCS(NULL)
{
}


void SceneCreator::createScene()
{

    this->createHead();
    this->createSky();

    this->setupCameraControlSystem();

}

void SceneCreator::createHead()
{
    // Create an Entity
    Ogre::Entity* headEntity = m_pSceneManager->createEntity("Head", "ogrehead.mesh");

    // Create a SceneNode and attach the Entity to it
    m_headNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
    m_headNode->attachObject(headEntity);
}

void SceneCreator::createSky()
{
//    Ogre::Root* root = Ogre::Root::getSingletonPtr();
//    m_caelumSystem = new Caelum::CaelumSystem (root, m_pSceneManager, Caelum::CaelumSystem::CAELUM_COMPONENT_SUN);

//    Ogre::Viewport* viewPort = m_pCamera->getViewport();
//    m_caelumSystem->attachViewport(viewPort);

//    root->addFrameListener (m_caelumSystem);

//    Caelum::CaelumPlugin::getSingleton().loadCaelumSystemFromScript (m_caelumSystem, "sky_system_name");
}

void SceneCreator::setupCameraControlSystem()
{
    // By default, Ogre::camera points to -Z

    // Create the camera system using the previously created ogre camera.
    m_pCameraCS = new CCS::CameraControlSystem(m_pSceneManager, "CameraControlSystem", m_pCamera);

    mFrameListener = new CameraControlSystemFrameListener(m_window, m_pCamera);
//    mFrameListener->showDebugOverlay(true);
    Ogre::Root::getSingletonPtr()->addFrameListener(mFrameListener);

    // -------------------------------------------------------------------------------------
    // Register a "Fixed" camera mode. In this mode the camera position and orientation
    // never change.

//    CCS::FixedCameraMode* camMode1;
//    camMode1 = new CCS::FixedCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("Fixed (2)",camMode1);
//    camMode1->setCameraPosition(Ogre::Vector3(-500,0,-500));
//    float roll = 0; float yaw = 225; float pitch = 10;
//    camMode1->setCameraOrientation( Ogre::Quaternion(Ogre::Radian(Ogre::Degree(roll)),Ogre::Vector3::UNIT_Z)
//        * Ogre::Quaternion(Radian(Degree(yaw)),Vector3::UNIT_Y)
//        * Ogre::Quaternion(Radian(Degree(pitch)),Vector3::UNIT_X));

    // -------------------------------------------------------------------------------------
    // Register another "Fixed" camera mode with a different camera position.

//    camMode1 = new CCS::FixedCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("Fixed",camMode1);
//    //camMode1->setCameraPosition(Ogre::Vector3(PLANE_SIZE,PLANE_SIZE,PLANE_SIZE));
//    camMode1->setCameraPosition(Ogre::Vector3(1500,500,1500));
//    roll = 0; yaw = 45; pitch = -20;
//    camMode1->setCameraOrientation( Quaternion(Radian(Degree(roll)),Vector3::UNIT_Z)
//        * Quaternion(Radian(Degree(yaw)),Vector3::UNIT_Y)
//        * Quaternion(Radian(Degree(pitch)),Vector3::UNIT_X));

    // -------------------------------------------------------------------------------------
    // Register a "FixedTracking" camera mode. In this mode the camera position is fixed
    // and the camera always points to the target.

//    CCS::FixedTrackingCameraMode* camMode2;
//    camMode2 = new CCS::FixedTrackingCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("FixedTracking",camMode2);
//    camMode2->setCameraPosition(Ogre::Vector3(500,0,-100));

    // -------------------------------------------------------------------------------------
    // Register a "Chase" camera mode with default tightness (0.01). In
    // this mode the camera follows the target. The second parameter is the relative position
    // to the target. The orientation of the camera is fixed by a yaw axis (UNIT_Y by default).

//    CCS::ChaseCameraMode* camMode3;
//    camMode3 = new CCS::ChaseCameraMode(m_pCameraCS,Ogre::Vector3(0,0,-200));
//    m_pCameraCS->registerCameraMode("Chase(0.01 tightness)",camMode3);

    // -------------------------------------------------------------------------------------
    // Register another "ChaseCameraMode" camera mode with max tightness value.

//    camMode3 = new CCS::ChaseCameraMode(m_pCameraCS,Ogre::Vector3(0,0,-200));
//    m_pCameraCS->registerCameraMode("Chase(0.2 tightness)",camMode3);
//    camMode3->setCameraTightness(0.2);

    // -------------------------------------------------------------------------------------
    // Register a "ChaseFreeYawAxis" camera mode with max tightness. This mode is
    // similar to "Chase" camera mode but the camera orientation is not fixed by
    // a yaw axis. The camera orientation will be the same as the target.

//    camMode3 = new CCS::ChaseFreeYawAxisCameraMode(m_pCameraCS,Ogre::Vector3(0,0,-200)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("ChaseFreeYawAxis(0.01 tightness)",camMode3);
//    camMode3->setCameraTightness(0.05);

    // -------------------------------------------------------------------------------------
    // Register another "ChaseFreeYawAxis" camera mode with 0.05 tightness value.

//    camMode3 = new CCS::ChaseFreeYawAxisCameraMode(m_pCameraCS,Ogre::Vector3(0,0,-200)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("ChaseFreeYawAxis(0.1 tightness)",camMode3);
//    camMode3->setCameraTightness(0.3);


    // -------------------------------------------------------------------------------------
    // Register a "FirstPerson" camera mode.

    CCS::FirstPersonCameraMode* camMode4 = new CCS::FirstPersonCameraMode(m_pCameraCS,Ogre::Vector3(0, 17, -100),
                                                                          Ogre::Radian(0),
                                                                          Ogre::Radian(Ogre::Degree(180)),
                                                                          Ogre::Radian(0));
    m_pCameraCS->registerCameraMode("FirstPerson", camMode4);

    // -------------------------------------------------------------------------------------
    // Register another "FirstPerson" camera mode where the character is hidden.

//    camMode4 = new CCS::FirstPersonCameraMode(m_pCameraCS,Ogre::Vector3(0,17,-16)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("FirstPerson (character hidden)",camMode4);
//    camMode4->setCharacterVisible(false);

    // -------------------------------------------------------------------------------------
    // Register a "PlaneBinded" camera mode. In this mode the camera is constrained to the
    // limits of a plane. The camera always points to the target, perpendicularly to the plane.

//    Ogre::Plane* mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Z, Ogre::Vector3(0,0,-200));
//    CCS::PlaneBindedCameraMode* camMode5 = new CCS::PlaneBindedCameraMode(m_pCameraCS, *mPlane);
//    m_pCameraCS->registerCameraMode("PlaneBinded (XY)",camMode5);

//    // -------------------------------------------------------------------------------------
//    // Register another "PlaneBinded" camera mode using a top point of view.

//    mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0,1000,0));
//    camMode5 = new CCS::PlaneBindedCameraMode(m_pCameraCS, *mPlane, Ogre::Vector3::UNIT_Z);
//    m_pCameraCS->registerCameraMode("PlaneBinded (XZ)",camMode5);

//    // -------------------------------------------------------------------------------------
//    // Register a "ThroughTarget" camera mode. In this mode the camera points to a given
//    // position (the "focus") throuh the target. The camera orientation is fixed by a yaw axis.

//    CCS::ThroughTargetCameraMode* camMode6 = new CCS::ThroughTargetCameraMode(m_pCameraCS, 400);
//    m_pCameraCS->registerCameraMode("ThroughTarget",camMode6);
//    camMode6->setCameraFocusPosition(atheneNode->_getDerivedPosition() - Ogre::Vector3(0,100,0));

//    // -------------------------------------------------------------------------------------
//    // Register another "ThroughTarget" camera mode. In this case the "focus" is the
//    // target and vice versa.

//    camMode6 = new CCS::ThroughTargetCameraMode(m_pCameraCS, 400, true);
//    m_pCameraCS->registerCameraMode("ThroughTarget (inverse)",camMode6);
//    camMode6->setCameraFocusPosition(atheneNode->_getDerivedPosition() + Ogre::Vector3(0,100,0));

//    // -------------------------------------------------------------------------------------
//    // Register a "ClosestToTarget" camera mode. In this camera mode the position of the
//    // camera is chosen to be the closest to the target of a given list. The camera
//    // orientation is fixed by a yaw axis.

//    CCS::ClosestToTargetCameraMode* camMode7 = new CCS::ClosestToTargetCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("ClosestToTarget",camMode7);

//    Ogre::Vector3* camPos1 = new Ogre::Vector3(-400,0,-400);
//    Ogre::Vector3* camPos2 = new Ogre::Vector3(-400,0,1400);
//    Ogre::Vector3* camPos3 = new Ogre::Vector3(1400,0,1400);

//    camMode7->addCameraPosition(*camPos1);
//    camMode7->addCameraPosition(*camPos2);
//    camMode7->addCameraPosition(*camPos3);

//    // -------------------------------------------------------------------------------------
//    // Register an "Attached" camera mode. In this mode the camera node is attached to the
//    // target node as a child.

//    CCS::AttachedCameraMode* camMode8 = new CCS::AttachedCameraMode(m_pCameraCS,Ogre::Vector3(200,0,0)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(90)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("Attached (lateral)",camMode8);

//    camMode8 = new CCS::AttachedCameraMode(m_pCameraCS,Ogre::Vector3(0,50,-200)
//        , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
//    m_pCameraCS->registerCameraMode("Attached (back/top)",camMode8);

//    // -------------------------------------------------------------------------------------
//    // Register a "Free" camera mode. In this mode the camera is controlled by the user.
//    // The camera orientation is fixed to a yaw axis.

//    CCS::FreeCameraMode* camMode9 = new CCS::FreeCameraMode(m_pCameraCS);
//    m_pCameraCS->registerCameraMode("Free",camMode9);
//    camMode9->setMoveFactor(30);

//    // -------------------------------------------------------------------------------------
//    // Register a "FixedDirection" camera mode. In this mode the
//    // target is always seen from the same point of view.

//    CCS::FixedDirectionCameraMode* camMode10 = new CCS::FixedDirectionCameraMode(m_pCameraCS, Ogre::Vector3(-1,-1,-1), 1000);
//    m_pCameraCS->registerCameraMode("Fixed direction",camMode10);

//    // -------------------------------------------------------------------------------------
//    // Register another "FixedDirection" camera mode with a 0.01 tightness value.

//    camMode10 = new CCS::FixedDirectionCameraMode(m_pCameraCS, Ogre::Vector3(-1,-1,-1), 1000);
//    m_pCameraCS->registerCameraMode("Fixed direction (0.01 tightness)",camMode10);
//    camMode10->setCameraTightness(0.01);

//    // -------------------------------------------------------------------------------------
//    // Register an "Orbital" camera mode. This is basically an attached camera mode where the user
//    // can mofify the camera position. If the scene focus is seen as the center of a sphere, the camera rotates arount it

//    CCS::OrbitalCameraMode* camMode12 = new CCS::OrbitalCameraMode(m_pCameraCS, 200);
//    m_pCameraCS->registerCameraMode("Orbital",camMode12);
//    camMode12->setZoomFactor(100);
//    camMode12->setRotationFactor(50);

//    // -------------------------------------------------------------------------------------
//    // Register a collidable orbital camera mode.
//    //

//    CCS::OrbitalCameraMode* camMode13 = new CCS::OrbitalCameraMode(m_pCameraCS, 200);
//    m_pCameraCS->registerCameraMode("Orbital (collidable)",camMode13);
//    camMode13->setZoomFactor(100);
//    camMode13->setRotationFactor(50);
//    camMode13->setZoom(400);
//    camMode13->setYaw(Radian(Degree(90)));
//    camMode13->setCollisionsEnabled(true);
//    // ** Uncomment for custom collisions calculation. By default the collisions are based on ogre basic raycast feature **
//    //camMode13->collisionDelegate = CCS::CollidableCamera::newCollisionDelegate(this
//    //	, &CameraControlSystemDemo::CustomCollisionDetectionFunction);


//    // -------------------------------------------------------------------------------------
//    // Register the custom "Dummy" camera mode defined previously. It basically goes forward
//    // and backward constantly

//    DummyCameraMode* camMode14 = new DummyCameraMode(m_pCameraCS, 400);
//    m_pCameraCS->registerCameraMode("Dummy",(CCS::CameraControlSystem::CameraMode*)camMode14);

    // Set the camera target
    m_pCameraCS->setCameraTarget(m_headNode);

    m_pCameraCS->setCurrentCameraMode(camMode4);
}
