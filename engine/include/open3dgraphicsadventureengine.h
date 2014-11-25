#ifndef OPEN3DGRAPHICSADVENTUREENGINE_H
#define OPEN3DGRAPHICSADVENTUREENGINE_H

class QWidget;
class SceneCreator;
class Open3DGraphicsAdventureEngineImpl;

class Open3DGraphicsAdventureEngine
{
public:
    Open3DGraphicsAdventureEngine(QWidget* parentWindow, SceneCreator *sceneCreator);
    ~Open3DGraphicsAdventureEngine();

    QWidget* getRenderWidget() const;

private:
    Open3DGraphicsAdventureEngineImpl* m_impl;
};

#endif // OPEN3DGRAPHICSADVENTUREENGINE_H
