#ifndef TESTPOINTSORT_H
#define TESTPOINTSORT_H

#include <QtTest/QTest>

#include "vertexsortinginformation.h"

class TestPointSort : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testSimple();

private:
    void checkPoints(const std::vector<VertexSortingInformation> &points);

    VertexSortingInformation m_upperLeft;
    VertexSortingInformation m_lowerLeft;
    VertexSortingInformation m_lowerRight;
    VertexSortingInformation m_upperRight;

};

#endif // TESTPOINTSORT_H
