#ifndef CHECKEDEDGE_H
#define CHECKEDEDGE_H

#include <QPair>
#include <OgreVector3.h>

class CheckedEdge
{
public:
    CheckedEdge();

    void reset();

    typedef QPair<bool, Ogre::Vector3> ValidPoint;

    ValidPoint start;
    ValidPoint end;

    inline friend std::ostream& operator <<
        ( std::ostream& o, const CheckedEdge& e )
    {
        o << "Edge(" << e.start.second << "; " << e.end.second << ")";
        return o;
    }

};

#endif // CHECKEDEDGE_H
