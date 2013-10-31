#include "ogrewidget.h"

#include <QDebug>
#include <QApplication>
#include <QResizeEvent>

#include "ogreutils.h"


#include "scenecreator.h"


OgreWidget::OgreWidget(QWidget *parent, Ogre::SceneManager *pSceneManager)
    : QWidget(parent),
      m_pSceneManager(pSceneManager),
      m_pCameraSceneNode(NULL),
      m_isInitialized(false),
      m_pOgreRenderWindow(NULL),
      m_pCamera(NULL)

{

}

void OgreWidget::update()
{
    if(m_isInitialized)
    {
        Ogre::Root::getSingleton()._fireFrameStarted();
        updateSceneManagersAfterMaterialsChange(); // :HACK: avoid ogre assert
        m_pOgreRenderWindow->update();
        Ogre::Root::getSingleton()._fireFrameRenderingQueued();
        Ogre::Root::getSingleton()._fireFrameEnded();
    }

}


void OgreWidget::initialize()
{
    // These attributes are the same as those use in a QGLWidget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Accept input focus
    setFocusPolicy(Qt::StrongFocus);

    // Parameters to pass to Ogre::Root::createRenderWindow()
    Ogre::NameValuePairList params;
    createOgreRenderWindowParameterList(this, params);


    // Finally create the window.
    Ogre::String renderWindowName = Ogre::String("OgreWidget");

    Ogre::String memoryAddress = Ogre::StringConverter::toString((unsigned long) ((void*) this));
    renderWindowName.append(memoryAddress);
    //    m_renderWindowName.append(foo);

    const unsigned int w = qMax(width(), 1);
    const unsigned int h = qMax(height(), 1);



    m_pOgreRenderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow(renderWindowName, w,
                                                                            h, false, &params);

    m_pOgreRenderWindow->setActive(true);
    m_pOgreRenderWindow->resize(w, h);
    m_pOgreRenderWindow->setVisible( true );

    // Attach render target
    Ogre::Root::getSingletonPtr()->getRenderSystem()->attachRenderTarget(*m_pOgreRenderWindow);

    this->initializeCamera(m_pSceneManager);
//    this->initializeLight(m_pSceneManager);


    m_isInitialized = true;


    m_creator = new SceneCreator(m_pSceneManager, m_pOgreRenderWindow, m_pCamera);
    m_creator->createScene();

    emit widgetInitialized();

}

void OgreWidget::initializeCamera(Ogre::SceneManager* sceneManager)
{
    Ogre::String memoryAddress = Ogre::StringConverter::toString((unsigned long) ((void*) this));

    Ogre::String camName = Ogre::String("Camera") + memoryAddress;
    Ogre::String camEntityName = Ogre::String("CameraEntity") + memoryAddress;

    Q_ASSERT(m_pCamera == NULL);
    m_pCamera = sceneManager->createCamera(camName);

    // hard code the cam view angle to ICameraController::s_kDEFAULT_FOV_DEGREES (50 degrees)
    Ogre::Radian fovy(70);

    m_pCamera->setFOVy(fovy);

//    const Ogre::Vector3 initialCamPos(0, 0, 500);

//    m_pCamera->setPosition(initialCamPos);
    m_pCamera->setNearClipDistance(1);
//    m_pCamera->setFarClipDistance(1E+6);
    m_pCamera->setFarClipDistance(99999*6);

//    const Ogre::Vector3 initialCamLookPos(0, 0, 0);

//    m_pCamera->lookAt(initialCamLookPos);
//    std::cout << m_pCamera->getOrientation() << std::endl;
//    std::cout << m_pCamera->getDirection() << std::endl;

//    Ogre::Quaternion orientation(Ogre::Radian(180), Ogre::Vector3::UNIT_Z);
//    Ogre::Quaternion orientation(0, 0, 0, 1);
//    m_pCamera->setOrientation(orientation);

//    m_pCamera->setDirection(1, 0, 0);

//    std::cout << m_pCamera->getOrientation() << std::endl;
//    std::cout << m_pCamera->getDirection() << std::endl;


    //    pLight->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    //    pLight->setSpecularColour(Ogre::ColourValue(0.5, 0.5, 0.5));

    //pLight->setDiffuseColour(Ogre::ColourValue(1.0f,0.0f,0.0f));
    //pLight->setSpecularColour(Ogre::ColourValue(1.0f,0.0f,0.0f));



//    Ogre::Frustum* pFrustum = new Ogre::Frustum;
//    pFrustum->setFOVy(fovy);
//    pFrustum->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
//    pFrustum->setNearClipDistance(1);
//    pFrustum->setFarClipDistance(1E+5);
//    m_pCamera->setCullingFrustum(pFrustum);

    // fog
    //    sceneManager->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(1.0, 1.0, 1.0), 0.0, 1000, 3000);

    m_pCameraSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(camEntityName);
//    m_pCameraSceneNode->attachObject(m_pCamera);

    // Add a default viewport and set the viewport's background color
    Ogre::Viewport* pVP = m_pOgreRenderWindow->addViewport(m_pCamera);
    pVP->setBackgroundColour(Ogre::ColourValue(0.1f, 0.2f, 0.3f));
    m_pCamera->setAspectRatio(Ogre::Real(pVP->getActualWidth()) / Ogre::Real(pVP->getActualHeight()));
    pVP->setOverlaysEnabled(false);

}

void OgreWidget::initializeLight(Ogre::SceneManager *sceneManager)
{
//    Ogre::Light* pLight = sceneManager->createLight("MainLight");
//    sceneManager->getRootSceneNode()->attachObject(pLight);
//    pLight->setType(Ogre::Light::LT_POINT);
//    pLight->setSpotlightInnerAngle(Ogre::Radian(Ogre::Degree(10)));
//    pLight->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(40)));
//    pLight->setSpotlightFalloff(0.2);


//    pLight->setPosition(1000, 1000, 1000);
//    pLight->setDirection(1, -1, 0);


    Ogre::Vector3 lightdir(0.55, -0.5, 0.75);
    lightdir.normalise();

    Ogre::Light* light = sceneManager->createLight("MainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

    sceneManager->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));

    sceneManager->getRootSceneNode()->attachObject(light);

}

void OgreWidget::resizeEvent(QResizeEvent* evt)
{
    if (m_pOgreRenderWindow)
    {
        quint32 width = evt->size().width();
        quint32 height = evt->size().height();
        qDebug() << "w:" << width << "h:" << height;
        //        qDebug() << m_pOgreRenderWindow->getWidth() << m_pOgreRenderWindow->getHeight();
        m_pOgreRenderWindow->resize(width, height);
        m_pOgreRenderWindow->windowMovedOrResized();

        // update ratios
        for (int ct = 0; ct < m_pOgreRenderWindow->getNumViewports(); ++ct)
        {
            Ogre::Viewport* pViewport = m_pOgreRenderWindow->getViewport(ct);
            //            qDebug() << pViewport->getWidth() << pViewport->getHeight();
            if(pViewport->getActualHeight() > 0 && pViewport->getActualWidth() > 0)
            {
                Ogre::Camera* pCamera = pViewport->getCamera();
                Ogre::Vector3 camPos = pCamera->getPosition();

                //                std::cout << camPos << std::endl;
//                this->m_cameraController->setCameraPosition(camPos.x, camPos.y, camPos.z);

                pCamera->setAspectRatio(static_cast<Ogre::Real>(pViewport->getActualWidth())
                                        / static_cast<Ogre::Real>(pViewport->getActualHeight()));
            }
        }


    }

}

void OgreWidget::showEvent(QShowEvent *event)
{
    if(!m_isInitialized)
    {
        QApplication::flush();
        //XFlush(QX11Info::display());
        //        initialize();
        //        this->show();
        this->initialize();
    }
    QWidget::showEvent(event);
}



