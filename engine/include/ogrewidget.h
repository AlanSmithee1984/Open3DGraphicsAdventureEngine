#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <QWidget>


namespace Ogre
{
class Camera;
class SceneManager;
class SceneNode;
class RenderWindow;
}

class OgreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OgreWidget(QWidget *parent, Ogre::SceneManager* pSceneManager);

    void update();


protected:
    virtual void resizeEvent(QResizeEvent *evt);

    virtual void showEvent(QShowEvent *event);
private:
    void initialize();

    void initializeLight(Ogre::SceneManager *sceneManager);
    void initializeCamera(Ogre::SceneManager *sceneManager);


    bool m_isInitialized;

    Ogre::SceneManager * m_pSceneManager;
    Ogre::SceneNode* m_pCameraSceneNode;

    Ogre::RenderWindow* m_pOgreRenderWindow;
    Ogre::Camera* m_pCamera;
//    Ogre::String m_renderWindowName;

signals:

public slots:

};

#endif // OGREWIDGET_H
