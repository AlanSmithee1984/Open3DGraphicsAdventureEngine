#ifndef SIMPLELINECOLLECTION_H
#define SIMPLELINECOLLECTION_H

#include "simpleline.h"

#include <QVector>
#include <OgreVector3.h>

class SimpleLineCollection
{
public:
    SimpleLineCollection(Ogre::SceneManager* sceneManager);

    void setLineData(const QList<SimpleLine::LineAttributes> &attributes);

private:
    void adjustLineNumber(quint32 numberOfDesiredLines);

    void addLine();
    void removeLine();

    Ogre::SceneManager* m_sceneManager;

    QVector<SimpleLine*> m_lineList;

};

#endif // SIMPLELINECOLLECTION_H
