#include "open3dgraphicsadventureengineimpl.h"

#include "ogreengine.h"
#include "ogrewidget.h"


Open3DGraphicsAdventureEngineImpl::Open3DGraphicsAdventureEngineImpl(QWidget* parentWidget)
{
    OgreEngine::instance()->initialize(parentWidget, 60);
    m_ogreWidget = OgreEngine::instance()->createOgreWidget(parentWidget);
    bool connectSuccess = connect(m_ogreWidget, SIGNAL(widgetInitialized()),
                                  this, SLOT(widgetInitialized()));
    Q_ASSERT(connectSuccess);

    m_ogreWidget->resize(parentWidget->size());

    m_ogreWidget->show();



}

void Open3DGraphicsAdventureEngineImpl::widgetInitialized()
{

}
