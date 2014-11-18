#ifndef TESTPOLYHEDRONVOLUMECALC_H
#define TESTPOLYHEDRONVOLUMECALC_H

#include <QTest>

class TestPolyhedronVolumeCalc : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testCase1();

private:

};

#endif // TESTPOLYHEDRONVOLUMECALC_H
