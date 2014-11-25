#include "open3dgraphicsadventureengine.h"

#include "open3dgraphicsadventureengineimpl.h"


Open3DGraphicsAdventureEngine::Open3DGraphicsAdventureEngine(QWidget *parentWindow, SceneCreator *sceneCreator)
{
    m_impl = new Open3DGraphicsAdventureEngineImpl(parentWindow, sceneCreator);
}

Open3DGraphicsAdventureEngine::~Open3DGraphicsAdventureEngine()
{
    delete m_impl;
}

QWidget *Open3DGraphicsAdventureEngine::getRenderWidget() const
{
    return m_impl->getRenderWidget();
}
