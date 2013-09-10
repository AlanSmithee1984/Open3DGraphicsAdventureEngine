#include "open3dgraphicsadventureengine.h"

#include "open3dgraphicsadventureengineimpl.h"


Open3DGraphicsAdventureEngine::Open3DGraphicsAdventureEngine(QWidget *parentWindow)
{
    m_impl = new Open3DGraphicsAdventureEngineImpl(parentWindow);
}

Open3DGraphicsAdventureEngine::~Open3DGraphicsAdventureEngine()
{
    delete m_impl;
}
