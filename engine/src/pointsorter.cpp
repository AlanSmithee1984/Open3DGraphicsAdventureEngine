#include "pointsorter.h"

bool PointSorter::less(const VertexSortingInformation &first, const VertexSortingInformation &second)
{
    const Ogre::Vector3 &center = first.center;
    const Ogre::Vector3 &normal = first.normal;

    const Ogre::Vector3 &a = first.point;
    const Ogre::Vector3 &b = second.point;

    const Ogre::Vector3 CA = a - center;
    const Ogre::Vector3 CB = b - center;

    const Ogre::Vector3 cross = CA.crossProduct(CB);

    const Ogre::Real projection = normal.dotProduct(cross);

    //    Q_ASSERT(qAbs(projection) >= std::numeric_limits<Ogre::Real>::epsilon());

    return projection > 0;
}

void PointSorter::sortPoints(std::vector<VertexSortingInformation> &points)
{
    std::sort(points.begin(), points.end(), less);
}

PointSorter::PointSorter()
{
}
