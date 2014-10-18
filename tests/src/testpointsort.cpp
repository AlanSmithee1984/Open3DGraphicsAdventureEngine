#include "testpointsort.h"

#include "pointsorter.h"
#include "polyhedronvolumecalculator.h"

#include <OgrePlane.h>

void TestPointSort::init()
{
    m_upperLeft.point = Ogre::Vector3(-1, 1, 1);
    m_lowerLeft.point = Ogre::Vector3(-1, -1, 1);
    m_lowerRight.point = Ogre::Vector3(1, -1, -1);
    m_upperRight.point = Ogre::Vector3(1, 1, -1);

    m_upperLeft.center = Ogre::Vector3::ZERO;
    m_lowerLeft.center = Ogre::Vector3::ZERO;
    m_lowerRight.center = Ogre::Vector3::ZERO;
    m_upperRight.center = Ogre::Vector3::ZERO;

    Ogre::Vector3 normal(1, 0, 1);
    normal.normalise();

    m_upperLeft.normal = normal;
    m_lowerLeft.normal = normal;
    m_lowerRight.normal = normal;
    m_upperRight.normal = normal;

}

void TestPointSort::cleanup()
{

}

void TestPointSort::testRightOrder()
{
    std::vector<VertexSortingInformation> points;

    points.push_back(m_upperLeft);
    points.push_back(m_lowerLeft);
    points.push_back(m_lowerRight);
    points.push_back(m_upperRight);

    QVERIFY(this->checkPoints(points) == true);

    PointSorter::sortPoints(points);

    QVERIFY(this->checkPoints(points) == true);

}

void TestPointSort::testReversed()
{
    std::vector<VertexSortingInformation> points;

    points.push_back(m_upperRight);
    points.push_back(m_lowerRight);
    points.push_back(m_lowerLeft);
    points.push_back(m_upperLeft);

    QVERIFY(this->checkPoints(points) == false);

    PointSorter::sortPoints(points);

    QVERIFY(this->checkPoints(points) == true);
}

void TestPointSort::testFirstAndSecondFlipped()
{
    std::vector<VertexSortingInformation> points;

    points.push_back(m_lowerLeft);
    points.push_back(m_upperLeft);
    points.push_back(m_lowerRight);
    points.push_back(m_upperRight);

    QVERIFY(this->checkPoints(points) == false);

    PointSorter::sortPoints(points);

    QVERIFY(this->checkPoints(points) == true);
}

bool TestPointSort::checkPoints(const std::vector<VertexSortingInformation> &points)
{
    Polygon poly;

    for(quint32 i = 0; i < points.size(); ++i)
    {
        Ogre::Vector3 vertexCurrent = points[i].point;
        poly.push_back(vertexCurrent);
    }

    const Ogre::Vector3 &normal = points.at(0).normal;

    const Ogre::Real area = PolyhedronVolumeCalculator::calcPolygonArea(poly, normal);

    return area > 0;
}

QTEST_MAIN(TestPointSort)
