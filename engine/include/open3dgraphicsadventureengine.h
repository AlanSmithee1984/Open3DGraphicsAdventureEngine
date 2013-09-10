#ifndef OPEN3DGRAPHICSADVENTUREENGINE_H
#define OPEN3DGRAPHICSADVENTUREENGINE_H

class QWidget;
class Open3DGraphicsAdventureEngineImpl;

class Open3DGraphicsAdventureEngine
{
public:
    Open3DGraphicsAdventureEngine(QWidget* parentWindow);
    ~Open3DGraphicsAdventureEngine();

private:
    Open3DGraphicsAdventureEngineImpl* m_impl;
};

#endif // OPEN3DGRAPHICSADVENTUREENGINE_H
