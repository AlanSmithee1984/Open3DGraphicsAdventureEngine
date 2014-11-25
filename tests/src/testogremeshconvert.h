#ifndef TESTOGREMESHCONVERT_H
#define TESTOGREMESHCONVERT_H

#include <QTest>

#include <OgreMesh.h>

class TestOgreMeshConvert : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void init();
    void cleanup();

    void cubeConvert();

private:
    Ogre::MeshPtr m_cubeMesh;
};

#endif // TESTOGREMESHCONVERT_H
