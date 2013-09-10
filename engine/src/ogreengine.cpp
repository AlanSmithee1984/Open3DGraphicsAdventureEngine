#include "ogreengine.h"

#include <QDebug>
#include <QTimer>

#include <OgreRoot.h>
#include <OgreConfigFile.h>

#include "ogreutils.h"

#include "ogrewidget.h"

OgreEngine::OgreEngine()
    : m_isInitialized(false)
{
}

void OgreEngine::update()
{
    foreach(OgreWidget* widget, m_widgets)
    {
        widget->update();
    }
}


void OgreEngine::initialize(QWidget *parentWidget, qreal fps)
{

    if(m_ogreLogMgr == NULL)
    {
        m_ogreLogMgr = OGRE_NEW Ogre::LogManager;
        m_ogreLogMgr->createLog("Ogre.log", true, false, false);
    }


    Ogre::Root* pRoot = Ogre::Root::getSingletonPtr();


    if (m_isInitialized == false) // once per application
    {
        //todo: activate assert later on
        //Q_ASSERT(pRoot == NULL);

        //fixme work around
        if (pRoot == NULL)
        {
            // OGRE initialization
            pRoot = new Ogre::Root("", "");

            //        pRoot->loadPlugin("Plugin_CgProgramManager");
            //pRoot->loadPlugin("Plugin_OctreeSceneManager");
            pRoot->loadPlugin("RenderSystem_GL");
        }
        // select opengl render system
        const Ogre::RenderSystemList& rList = Ogre::Root::getSingletonPtr()->getAvailableRenderers();
        Ogre::RenderSystemList::const_iterator it = rList.begin();
        Ogre::RenderSystem *rSys = NULL;

        while (it != rList.end())
        {
            rSys = *(it++);
            std::cout << rSys->getName() << std::endl;

            if (rSys->getName().find("OpenGL") != std::string::npos)
            {
                pRoot->setRenderSystem(rSys);
                break;
            }
        }

        Q_ASSERT(rSys);

        pRoot->initialise(false);

        // load built-in resources
        Ogre::MaterialManager::getSingleton().initialise();



        const Ogre::String ogreResourceFile = "resources.cfg";
        const Ogre::String ogreResourcePath = "./";
        const Ogre::String filePath = ogreResourcePath + ogreResourceFile;

        // Load resource paths from config file
        Ogre::ConfigFile cf;

        cf.load(filePath);


        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = ogreResourcePath + i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }

        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // Parameters to pass to Ogre::Root::createRenderWindow()
        Ogre::NameValuePairList params;

        createOgreRenderWindowParameterList(parentWidget, params);

        // Finally create the window.
        Ogre::String renderWindowName = "OgreEngineHiddenRenderWindow";


        m_pOgreRenderWindow = pRoot->createRenderWindow(renderWindowName, 1, 1, false, &params);

    }
    Q_ASSERT(pRoot != NULL);


    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(1000.0 / fps);

    bool connectSuccess = connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
    Q_ASSERT(connectSuccess);

    m_pTimer->start();


    m_totalTimeAvailable = 1000.0 / fps;

//    GLint size[3];
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE, size);
//    qDebug() << size[0] << size[1] << size[2];


    this->initializeSceneManager();

    m_isInitialized = true;



}

OgreWidget *OgreEngine::createOgreWidget(QWidget* parent)
{
    OgreWidget* widget = new OgreWidget(parent, m_pSceneManager);
    m_widgets << widget;
    return widget;
}

void OgreEngine::initializeSceneManager()
{
    m_pSceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC);

    //    // Extend the size of the space managed by the scene manager
    Ogre::String optionSize = "Size";

    // Set the Size of the projected earth for the x-y-Plane as approximation
    // FIXME: hardcode the size here
    Ogre::AxisAlignedBox newSize(-10, -10, -10, 10, 10, 10);
    m_pSceneManager->setOption(optionSize, &newSize);
    //qDebug() << "new managed scene size" << newSize.getSize().x << newSize.getSize().y << newSize.getSize().z;

    m_pSceneManager->getRootSceneNode()->setVisible(true);

//    m_pRenderingPipelineConfigurator = new RenderingPipelineConfigurator(pSceneManager);


}
