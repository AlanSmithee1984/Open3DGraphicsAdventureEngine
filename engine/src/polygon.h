#ifndef POLYGON_H
#define POLYGON_H

#include <OgreVector3.h>
#include <QList>

typedef std::vector<Ogre::Vector3> Polygon;

class Polygons : public QList<Polygon>
{
public:
    Polygons();

private:

};

#endif // POLYGON_H
