#include "testpolyhedronvolumecalc.h"

#include "polyhedronvolumecalculator.h"
#include "vertexinformation.h"

#include <QDebug>

void TestPolyhedronVolumeCalc::init()
{
    m_infos = this->constructCube();
}

void TestPolyhedronVolumeCalc::cleanup()
{
    foreach(VertexInformation* info, m_infos)
    {
        delete info;
    }
}

void TestPolyhedronVolumeCalc::testCubeVolume()
{

    Ogre::Real result = PolyhedronVolumeCalculator::calcPolyhedronVolume(m_infos);

    qDebug() << result;

    QCOMPARE(result, 8.0f);

}

void TestPolyhedronVolumeCalc::benchmarkCubeVolumes()
{

    QBENCHMARK
    {
        Ogre::Real result = PolyhedronVolumeCalculator::calcPolyhedronVolume(m_infos);
    }


}

VertexInformations TestPolyhedronVolumeCalc::constructCube() const
{
    const Ogre::Vector3 lowerLeftFront(-1, -1, -1);
    const Ogre::Vector3 lowerRightFront(1, -1, -1);
    const Ogre::Vector3 upperRightFront(1, -1, 1);
    const Ogre::Vector3 upperLeftFront(-1, -1, 1);

    const Ogre::Vector3 lowerLeftBack(-1, 1, -1);
    const Ogre::Vector3 lowerRightBack(1, 1, -1);
    const Ogre::Vector3 upperRightBack(1, 1, 1);
    const Ogre::Vector3 upperLeftBack(-1, 1, 1);

    VertexInformations infos;

    // lower left front
    infos << VertexInformation::construct(lowerLeftFront,
                                          lowerRightFront,
                                          upperLeftFront);

    infos << VertexInformation::construct(lowerLeftFront,
                                          upperLeftFront,
                                          lowerLeftBack);

    infos << VertexInformation::construct(lowerLeftFront,
                                          lowerLeftBack,
                                          lowerRightFront);

    // lower right front
    infos << VertexInformation::construct(lowerRightFront,
                                          upperRightFront,
                                          lowerLeftFront);

    infos << VertexInformation::construct(lowerRightFront,
                                          lowerRightBack,
                                          upperRightFront);

    infos << VertexInformation::construct(lowerRightFront,
                                          lowerLeftFront,
                                          lowerRightBack);


    // upper right front
    infos << VertexInformation::construct(upperRightFront,
                                          upperLeftFront,
                                          lowerRightFront);

    infos << VertexInformation::construct(upperRightFront,
                                          upperRightBack,
                                          upperLeftFront);

    infos << VertexInformation::construct(upperRightFront,
                                          lowerRightFront,
                                          upperRightBack);

    // upper left front
    infos << VertexInformation::construct(upperLeftFront,
                                          lowerLeftFront,
                                          upperRightFront);

    infos << VertexInformation::construct(upperLeftFront,
                                          upperLeftBack,
                                          lowerLeftFront);

    infos << VertexInformation::construct(upperLeftFront,
                                          upperRightFront,
                                          upperLeftBack);




    // upper right back
    infos << VertexInformation::construct(upperRightBack,
                                          lowerRightBack,
                                          upperLeftBack);

    infos << VertexInformation::construct(upperRightBack,
                                          upperRightFront,
                                          lowerRightBack);

    infos << VertexInformation::construct(upperRightBack,
                                          upperLeftBack,
                                          upperRightFront);

    // upper left back
    infos << VertexInformation::construct(upperLeftBack,
                                          upperRightBack,
                                          lowerLeftBack);

    infos << VertexInformation::construct(upperLeftBack,
                                          upperLeftFront,
                                          upperRightBack);

    infos << VertexInformation::construct(upperLeftBack,
                                          lowerLeftBack,
                                          upperLeftFront);

    // lower left back
    infos << VertexInformation::construct(lowerLeftBack,
                                          upperLeftBack,
                                          lowerRightBack);

    infos << VertexInformation::construct(lowerLeftBack,
                                          lowerLeftFront,
                                          upperLeftBack);

    infos << VertexInformation::construct(lowerLeftBack,
                                          lowerRightBack,
                                          lowerLeftFront);

    // lower right back
    infos << VertexInformation::construct(lowerRightBack,
                                          lowerLeftBack,
                                          upperRightBack);

    infos << VertexInformation::construct(lowerRightBack,
                                          upperRightBack,
                                          lowerRightFront);

    infos << VertexInformation::construct(lowerRightBack,
                                          lowerRightFront,
                                          lowerLeftBack);

    return infos;
}

QTEST_MAIN(TestPolyhedronVolumeCalc)
