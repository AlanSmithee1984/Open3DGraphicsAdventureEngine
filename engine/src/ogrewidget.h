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

class SceneCreator;

class OgreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OgreWidget(QWidget *parent, Ogre::SceneManager* pSceneManager);

    void initialize();
    void update();




protected:
    virtual void resizeEvent(QResizeEvent *evt);

    virtual void showEvent(QShowEvent *event);

    virtual void keyPressEvent(QKeyEvent* event);

private:

    void initializeScene();

    void initializeCamera(Ogre::SceneManager *sceneManager);

    Ogre::SceneManager * m_pSceneManager;
    Ogre::SceneNode* m_pCameraSceneNode;
    bool m_isInitialized;

    Ogre::RenderWindow* m_pOgreRenderWindow;
    Ogre::Camera* m_pCamera;

    SceneCreator* m_creator;

signals:
    void widgetInitialized();

private slots:

};

#endif // OGREWIDGET_H
