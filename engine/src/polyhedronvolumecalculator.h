#ifndef POLYHEDRONVOLUMECALCULATOR_H
#define POLYHEDRONVOLUMECALCULATOR_H

#include "polygon.h"

class PolyhedronVolumeCalculator
{
public:

    static Ogre::Real calcPolyhedronVolume(const Polygons &faces);


private:
    PolyhedronVolumeCalculator();
};

#endif // POLYHEDRONVOLUMECALCULATOR_H
