#ifndef TESTPOLYHEDRONCLIP_H
#define TESTPOLYHEDRONCLIP_H

#include <QTest>

#include <OgreMesh.h>

class TestPolyhedronClip : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void init();
    void cleanup();

    void testPyramideClip();
//    void testCubeClip();


private:
    Ogre::MeshPtr m_pyramideMesh;
    Ogre::MeshPtr m_cubeMesh;
};

#endif // TESTPOLYHEDRONCLIP_H
