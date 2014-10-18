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

void TestPointSort::testSimple()
{
    std::vector<VertexSortingInformation> points;

    points.push_back(m_upperLeft);
    points.push_back(m_lowerLeft);
    points.push_back(m_lowerRight);
    points.push_back(m_upperRight);

    this->checkPoints(points);


    PointSorter::sortPoints(points);

    this->checkPoints(points);




}

void TestPointSort::checkPoints(const std::vector<VertexSortingInformation> &points)
{
    Polygon poly;

    for(quint32 i = 0; i < points.size(); ++i)
    {
        Ogre::Vector3 vertexCurrent = points[i].point;
        poly.push_back(vertexCurrent);

    }

    Q_ASSERT(poly.size() >= 3);
    const Ogre::Plane plane(poly[0], poly[1], poly[2]);

    Ogre::Real area = PolyhedronVolumeCalculator::calcPolygonArea(poly, plane.normal);

    QVERIFY(area >= 0);
}

QTEST_MAIN(TestPointSort)
