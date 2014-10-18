#ifndef POINTSORTER_H
#define POINTSORTER_H

#include <vertexsortinginformation.h>

class PointSorter
{
public:
    static void sortPoints(std::vector<VertexSortingInformation> &points);


private:
    PointSorter();


    static bool less(const VertexSortingInformation &first,
                     const VertexSortingInformation &second);
};

#endif // POINTSORTER_H
