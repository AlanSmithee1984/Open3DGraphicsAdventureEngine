#include "testogremeshconvert.h"

#include <OgreManualObject.h>
#include "ogreengine.h"
#include "meshtriangleconverter.h"
#include "polyhedron.h"

void TestOgreMeshConvert::initTestCase()
{
    OgreEngine::instance()->initialize();

    OgreWidget* w = OgreEngine::instance()->createOgreWidget(NULL);

}

void TestOgreMeshConvert::init()
{
    Ogre::ManualObject manualObject("bla");
    manualObject.begin("foo");

//    manualObject.position(-1, 0, 0);
//    manualObject.position(0, -1, 0);
//    manualObject.position(1, 0, 0);
//    manualObject.position(0, 0, 1);

//    manualObject.triangle(1, 0, 2);
//    manualObject.triangle(0, 1, 3);
//    manualObject.triangle(1, 2, 3);
//    manualObject.triangle(2, 0, 3);

    const Ogre::Vector3 lowerLeftFront(-1, -1, -1);
    const Ogre::Vector3 lowerRightFront(1, -1, -1);
    const Ogre::Vector3 upperRightFront(1, -1, 1);
    const Ogre::Vector3 upperLeftFront(-1, -1, 1);

    const Ogre::Vector3 lowerLeftBack(-1, 1, -1);
    const Ogre::Vector3 lowerRightBack(1, 1, -1);
    const Ogre::Vector3 upperRightBack(1, 1, 1);
    const Ogre::Vector3 upperLeftBack(-1, 1, 1);

    manualObject.position(lowerLeftFront);
    manualObject.position(lowerRightFront);
    manualObject.position(upperRightFront);
    manualObject.position(upperLeftFront);

    manualObject.position(lowerLeftBack);
    manualObject.position(lowerRightBack);
    manualObject.position(upperRightBack);
    manualObject.position(upperLeftBack);

    // front
    manualObject.quad(0, 1, 2, 3);

    // back
    manualObject.quad(7, 6, 5, 4);

    // top
    manualObject.quad(3, 2, 6, 7);

    // bottom
    manualObject.quad(1, 0, 4, 5);

    // right
    manualObject.quad(1, 5, 6, 2);

    // left
    manualObject.quad(4, 0, 3, 7);

    manualObject.end();

    m_cubeMesh = manualObject.convertToMesh("test");
}

void TestOgreMeshConvert::cleanup()
{

}

void TestOgreMeshConvert::cubeConvert()
{
    Polyhedron* poly = MeshTriangleConverter::convert(m_cubeMesh);

    const quint32 nFaces = poly->getNumberFaces();
    QVERIFY(nFaces == 12);

    const quint32 nHalfEdges = poly->getNumberHalfEdges();
    QVERIFY(nHalfEdges == (8 + 4 + 6) * 2);

    const quint32 nVertices = poly->getNumberVertices();
    QVERIFY(nVertices == 8);

    QVERIFY(poly->getLocalVertexInformations().size() == 12 * 3);

}

QTEST_MAIN(TestOgreMeshConvert)
