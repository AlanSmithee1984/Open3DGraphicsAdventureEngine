#include "testogremeshconvert.h"

#include <OgreManualObject.h>
#include "ogreengine.h"
#include "meshtriangleconverter.h"

void TestOgreMeshConvert::initTestCase()
{
    OgreEngine::instance()->initialize();

    OgreWidget* w = OgreEngine::instance()->createOgreWidget(NULL);

}

void TestOgreMeshConvert::init()
{
    Ogre::ManualObject manualObject("bla"); // man = new Ogre::ManualObject("bla");
    manualObject.begin("foo");

    manualObject.position(-1, 0, 0);
    manualObject.position(0, -1, 0);
    manualObject.position(1, 0, 0);
    manualObject.position(0, 0, 1);

    manualObject.triangle(1, 0, 2);
    manualObject.triangle(0, 1, 3);
    manualObject.triangle(1, 2, 3);
    manualObject.triangle(2, 0, 3);

    manualObject.end();

    m_cubeMesh = manualObject.convertToMesh("test");
}

void TestOgreMeshConvert::cleanup()
{

}

void TestOgreMeshConvert::cubeConvert()
{
    Polyhedron* poly = MeshTriangleConverter::convert(m_cubeMesh);

}

QTEST_MAIN(TestOgreMeshConvert)
