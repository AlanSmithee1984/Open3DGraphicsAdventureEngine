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
      m_pCamera(NULL),
      m_creator(NULL)

{

}

void OgreWidget::update()
{
    if(m_isInitialized)
    {
//        Ogre::Root::getSingleton()._fireFrameStarted();
//        updateSceneManagersAfterMaterialsChange(); // :HACK: avoid ogre assert
//        m_pOgreRenderWindow->update();
//        Ogre::Root::getSingleton()._fireFrameRenderingQueued();
//        Ogre::Root::getSingleton()._fireFrameEnded();

        Ogre::WindowEventUtilities::messagePump();

        Ogre::Root::getSingletonPtr()->renderOneFrame();
    }


}

void OgreWidget::initializeScene()
{
    if(m_creator == NULL)
    {
        m_creator = new SceneCreator(m_pSceneManager, m_pOgreRenderWindow, m_pCamera);
        m_creator->createScene();
    }
}


void OgreWidget::initialize()
{
    // These attributes are the same as those use in a QGLWidget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    this->setSizePolicy(QSizePolicy::Ignored,
                        QSizePolicy::Ignored);

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

    // do it in scene creator
//    this->initializeLight(m_pSceneManager);


    m_isInitialized = true;



    emit widgetInitialized();

}

void OgreWidget::initializeCamera(Ogre::SceneManager* sceneManager)
{
    Ogre::String memoryAddress = Ogre::StringConverter::toString((unsigned long) ((void*) this));

    Ogre::String camName = Ogre::String("Camera") + memoryAddress;
    Ogre::String camEntityName = Ogre::String("CameraEntity") + memoryAddress;

    Q_ASSERT(m_pCamera == NULL);
    m_pCamera = sceneManager->createCamera(camName);

//    Ogre::Radian fovy(70);
//    m_pCamera->setFOVy(fovy);

    m_pCamera->setNearClipDistance(1.0f);
    m_pCamera->setFarClipDistance(100000);

    m_pCameraSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(camEntityName);

    // Add a default viewport and set the viewport's background color
    Ogre::Viewport* pVP = m_pOgreRenderWindow->addViewport(m_pCamera);
//    pVP->setBackgroundColour(Ogre::ColourValue(0.1f, 0.2f, 0.3f));
    m_pCamera->setAspectRatio(Ogre::Real(pVP->getActualWidth()) / Ogre::Real(pVP->getActualHeight()));
//    pVP->setOverlaysEnabled(false);

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



