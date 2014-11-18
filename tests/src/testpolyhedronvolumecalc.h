#ifndef TESTPOLYHEDRONVOLUMECALC_H
#define TESTPOLYHEDRONVOLUMECALC_H

#include <QTest>

#include "vertexinformation.h";

class TestPolyhedronVolumeCalc : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testCubeVolume();
    void benchmarkCubeVolumes();

private:
    VertexInformations constructCube() const;
};

#endif // TESTPOLYHEDRONVOLUMECALC_H
