#ifndef EDGE_H
#define EDGE_H

#include <OgreVector3.h>

class Edge
{
public:
    Edge();
    Edge(const Ogre::Vector3 &start,
         const Ogre::Vector3 &end);

    Ogre::Vector3 start;
    Ogre::Vector3 end;

    inline friend std::ostream& operator <<
        ( std::ostream& o, const Edge& e )
    {
        o << "Edge(" << e.start << " -> " << e.end << ")";
        return o;
    }


};

#endif // EDGE_H
