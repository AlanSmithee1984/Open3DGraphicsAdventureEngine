#include "open3dgraphicsadventureengineimpl.h"

#include "ogreengine.h"
#include "ogrewidget.h"


Open3DGraphicsAdventureEngineImpl::Open3DGraphicsAdventureEngineImpl(QWidget* parentWidget, SceneCreator* sceneCreator)
{
    OgreEngine::instance()->initialize(60);
    m_ogreWidget = OgreEngine::instance()->createOgreWidget(parentWidget);
    m_ogreWidget->setSceneCreator(sceneCreator);

    bool connectSuccess = connect(m_ogreWidget, SIGNAL(widgetInitialized()),
                                  this, SLOT(widgetInitialized()));
    Q_ASSERT(connectSuccess);

    m_ogreWidget->resize(parentWidget->size());

//    m_ogreWidget->initialize();



}

QWidget *Open3DGraphicsAdventureEngineImpl::getRenderWidget() const
{
    return m_ogreWidget;
}

void Open3DGraphicsAdventureEngineImpl::widgetInitialized()
{
    m_ogreWidget->show();
}
