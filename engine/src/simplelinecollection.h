#ifndef SIMPLELINECOLLECTION_H
#define SIMPLELINECOLLECTION_H

#include "simpleline.h"

#include <QVector>
#include <OgreVector3.h>

class SimpleLineCollection
{
public:
    SimpleLineCollection(Ogre::SceneManager* sceneManager);

    void addAxis();
    void addLine();

    void removeAxis();
    void removeLine();

    void setLineData(const QList<SimpleLine::LineAttributes> &attributes);

private:
    Ogre::SceneManager* m_sceneManager;

    QVector<SimpleLine*> m_lineList;

};

#endif // SIMPLELINECOLLECTION_H
