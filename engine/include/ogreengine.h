#ifndef OGREENGINE_H
#define OGREENGINE_H

#include "singleton.h"
#include <QObject>

class QTimer;
class QWidget;

class OgreWidget;

namespace Ogre
{
class RenderWindow;
class LogManager;
class SceneManager;
}

class OgreEngine : public QObject, public Singleton<OgreEngine>
{
    Q_OBJECT
public:
    void initialize(QWidget* parentWidget, qreal fps = 60.0);

    OgreWidget* createOgreWidget(QWidget *parent);

private:
    friend class Singleton<OgreEngine>;
    OgreEngine();

    void initializeSceneManager();

    quint32 m_totalTimeAvailable;
    QTimer* m_pTimer;

    Ogre::RenderWindow* m_pOgreRenderWindow;
    Ogre::LogManager* m_ogreLogMgr;

    bool m_isInitialized;

    QList<OgreWidget*> m_widgets;


    Ogre::SceneManager * m_pSceneManager;

private slots:
    void update();


};

#endif // OGREENGINE_H
