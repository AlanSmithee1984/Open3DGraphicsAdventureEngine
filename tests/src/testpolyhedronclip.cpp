#include "testpolyhedronclip.h"

#include "ogreengine.h"

#include <OgreManualObject.h>

#include "polyhedron.h"
#include "meshtriangleconverter.h"
#include "polyhedronvolumecalculator.h"

void TestPolyhedronClip::initTestCase()
{
    OgreEngine::instance()->initialize();

    OgreWidget* w = OgreEngine::instance()->createOgreWidget(NULL);

    Ogre::ManualObject cubeManualObject("cube");
    cubeManualObject.begin("foo");

    const Ogre::Vector3 lowerLeftFront(-1, -1, -1);
    const Ogre::Vector3 lowerRightFront(1, -1, -1);
    const Ogre::Vector3 upperRightFront(1, -1, 1);
    const Ogre::Vector3 upperLeftFront(-1, -1, 1);

    const Ogre::Vector3 lowerLeftBack(-1, 1, -1);
    const Ogre::Vector3 lowerRightBack(1, 1, -1);
    const Ogre::Vector3 upperRightBack(1, 1, 1);
    const Ogre::Vector3 upperLeftBack(-1, 1, 1);

    cubeManualObject.position(lowerLeftFront);
    cubeManualObject.position(lowerRightFront);
    cubeManualObject.position(upperRightFront);
    cubeManualObject.position(upperLeftFront);

    cubeManualObject.position(lowerLeftBack);
    cubeManualObject.position(lowerRightBack);
    cubeManualObject.position(upperRightBack);
    cubeManualObject.position(upperLeftBack);

    // front
    cubeManualObject.quad(0, 1, 2, 3);

    // back
    cubeManualObject.quad(7, 6, 5, 4);

    // top
    cubeManualObject.quad(3, 2, 6, 7);

    // bottom
    cubeManualObject.quad(1, 0, 4, 5);

    // right
    cubeManualObject.quad(1, 5, 6, 2);

    // left
    cubeManualObject.quad(4, 0, 3, 7);

    cubeManualObject.end();

    m_cubeMesh = cubeManualObject.convertToMesh("cubeMesh");



    Ogre::ManualObject pyramideManualObject("cube");
    pyramideManualObject.begin("foo");

    pyramideManualObject.position(-1, 0, 0);
    pyramideManualObject.position(0, -1, 0);
    pyramideManualObject.position(1, 0, 0);
    pyramideManualObject.position(0, 0, 1);

    pyramideManualObject.triangle(1, 0, 2);
    pyramideManualObject.triangle(0, 1, 3);;
    pyramideManualObject.triangle(1, 2, 3);
    pyramideManualObject.triangle(2, 0, 3);

    pyramideManualObject.end();

    m_pyramideMesh = pyramideManualObject.convertToMesh("pyramideMesh");

}

void TestPolyhedronClip::init()
{


}

void TestPolyhedronClip::cleanup()
{

}

void TestPolyhedronClip::testPyramideClip()
{
    Polyhedron* poly = MeshTriangleConverter::convert(m_pyramideMesh);

    const quint32 nFaces = poly->getNumberFaces();
    QVERIFY(nFaces == 4);

    const quint32 nHalfEdges = poly->getNumberHalfEdges();
    QVERIFY(nHalfEdges == 12);

    const quint32 nVertices = poly->getNumberVertices();
    QVERIFY(nVertices == 4);

    VertexInformations infosBeforeClip = poly->getLocalVertexInformations();

    QVERIFY(infosBeforeClip.size() == 4 * 3);
    Ogre::Real volBeforeClip = PolyhedronVolumeCalculator::calcPolyhedronVolume(infosBeforeClip);
    QCOMPARE(volBeforeClip, 1.0f/3.0f);



    Ogre::Plane pl(Ogre::Vector3::UNIT_Z, 0.5);

    VertexInformations infos;
    poly->clipAtPlane(pl, infos);

    QVERIFY(infos.size() == 6 * 3);

    Ogre::Real vol = PolyhedronVolumeCalculator::calcPolyhedronVolume(infos);

    qDebug() << vol;
    QCOMPARE(vol, 0.2916666667f);
}

//void TestPolyhedronClip::testCubeClip()
//{
//    Polyhedron* poly = MeshTriangleConverter::convert(m_cubeMesh);

//    const quint32 nFaces = poly->getNumberFaces();
//    QVERIFY(nFaces == 12);

//    const quint32 nHalfEdges = poly->getNumberHalfEdges();
//    QVERIFY(nHalfEdges == (8 + 4 + 6) * 2);

//    const quint32 nVertices = poly->getNumberVertices();
//    QVERIFY(nVertices == 8);

//    VertexInformations infosBeforeClip = poly->getLocalVertexInformations();

//    // 8 -> each vertex
//    // 4 -> each second vertex has doubled edges
//    QVERIFY(infosBeforeClip.size() == 8 * 3 + 4 * 3);
//    Ogre::Real volBeforeClip = PolyhedronVolumeCalculator::calcPolyhedronVolume(infosBeforeClip);
//    qDebug() << "before clip:" << volBeforeClip;
//    QCOMPARE(volBeforeClip, 8.0f);



//    Ogre::Plane pl(Ogre::Vector3::UNIT_Z, 0.5);

//    VertexInformations infos;
//    poly->clipAtPlane(pl, infos);

//    QVERIFY(infos.size() == 8 * 3 + 4 * 3);

//    Ogre::Real vol = PolyhedronVolumeCalculator::calcPolyhedronVolume(infos);

//    qDebug() << vol;
//    QCOMPARE(vol, 6.0f);
//}

QTEST_MAIN(TestPolyhedronClip)
