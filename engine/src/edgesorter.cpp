#include "edgesorter.h"

#include <QVector>

EdgeSorter::EdgeSorter()
{
}

Polygon EdgeSorter::sortEdges(QList<Edge> &edges)
{
    Polygon resultingPoly;

    const Edge firstEdge = edges.takeFirst();

    resultingPoly.push_back(firstEdge.start);
    resultingPoly.push_back(firstEdge.end);


    while(edges.isEmpty() == false)
    {
        Ogre::Vector3 result = extractEdgeWithStart(edges, resultingPoly.back());

        if(edges.isEmpty() == false)
        {
            resultingPoly.push_back(result);
        }
        else
        {
            Q_ASSERT(compare(result, resultingPoly.front()));
        }
    }


    return resultingPoly;
}



Ogre::Vector3 EdgeSorter::extractEdgeWithStart(QList<Edge> &edges, const Ogre::Vector3 &start)
{
    for(QList<Edge>::Iterator it = edges.begin() ; it != edges.end(); ++it)
    {
        Edge edge = *it;

        if(compare(edge.start, start))
        {
            edges.erase(it);
            std::cout << "extracting edge (end): " << edge << "\t" << edges.size() << " left" << std::endl;
            return edge.end;
        }
        else if(compare(edge.end, start))
        {
            edges.erase(it);
            std::cout << "extracting edge (start): " << edge << "\t" << edges.size() << " left" << std::endl;
            return edge.start;
        }
    }

    Q_ASSERT(false);

    return Ogre::Vector3::ZERO;
}

bool EdgeSorter::compare(const Ogre::Vector3 &a, const Ogre::Vector3 &b)
{
    Ogre::Real squDist = a.squaredDistance(b);
    bool isZeroLength = squDist < std::numeric_limits<Ogre::Real>::epsilon();

    return isZeroLength;
}
