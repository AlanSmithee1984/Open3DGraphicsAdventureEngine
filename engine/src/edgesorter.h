#ifndef EDGESORTER_H
#define EDGESORTER_H

#include "polygon.h"
#include "edge.h"

class EdgeSorter
{
public:
    static Polygon sortEdges(QList<Edge> &edges);


private:
    EdgeSorter();

    static Ogre::Vector3 extractEdgeWithStart(QList<Edge> &edges, const Ogre::Vector3 &start);

    static bool compare(const Ogre::Vector3 &a, const Ogre::Vector3 &b);


};

#endif // EDGESORTER_H
