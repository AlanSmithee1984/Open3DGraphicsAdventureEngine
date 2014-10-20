#include "edgesorter.h"

#include <QVector>

EdgeSorter::EdgeSorter()
{
}

Polygon EdgeSorter::sortEdges(QList<Edge> &edges)
{
    Polygon resultingPoly;

    const Edge &firstEdge = edges.front();

    resultingPoly.push_back(firstEdge.start);
    resultingPoly.push_back(firstEdge.end);
    edges.pop_front();

    while(edges.isEmpty() == false)
    {
        Edge result = extractEdgeWithStart(edges, resultingPoly.back());

        if(edges.isEmpty() == false)
        {
            resultingPoly.push_back(result.end);
        }
        else
        {
            Q_ASSERT(compare(result.end, resultingPoly.front()));
        }
    }


    return resultingPoly;
}



Edge EdgeSorter::extractEdgeWithStart(QList<Edge> &edges, const Ogre::Vector3 &start)
{
    for(QList<Edge>::Iterator it = edges.begin() ; it != edges.end(); ++it)
    {
        Edge edge = *it;

        if(compare(edge.start, start))
        {
            edges.erase(it);
            std::cout << "extracting edge: " << edge << "\t" << edges.size() << " left" << std::endl;
            return edge;
        }
    }

//    Q_ASSERT(false);

    if(edges.size() > 1)
    {
        return edges.takeFirst();
    }

    return Edge();
}

bool EdgeSorter::compare(const Ogre::Vector3 &a, const Ogre::Vector3 &b)
{
    const Ogre::Vector3 vec = a - b;
    bool isZeroLength = vec.squaredLength() < 1E-5;

    return isZeroLength;
}
