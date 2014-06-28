#ifndef OPEN3DGRAPHICSADVENTUREENGINEIMPL_H
#define OPEN3DGRAPHICSADVENTUREENGINEIMPL_H

#include <QObject>

class OgreWidget;

class Open3DGraphicsAdventureEngineImpl : public QObject
{
    Q_OBJECT
public:
    Open3DGraphicsAdventureEngineImpl(QWidget* parentWidget);

    QWidget *getRenderWidget() const;

private:
    OgreWidget* m_ogreWidget;

private slots:
    void widgetInitialized();
};

#endif // OPEN3DGRAPHICSADVENTUREENGINEIMPL_H
