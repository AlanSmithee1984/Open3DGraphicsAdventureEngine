#include "simplelinecollection.h"

#include <QDebug>

SimpleLineCollection::SimpleLineCollection(Ogre::SceneManager* sceneManager)
    : m_sceneManager(sceneManager)
{
}

void SimpleLineCollection::addAxis()
{
    this->addLine();
    this->addLine();
    this->addLine();
}

void SimpleLineCollection::addLine()
{
    SimpleLine* line = new SimpleLine(m_sceneManager);

    m_lineList << line;
}

void SimpleLineCollection::removeAxis()
{
    this->removeLine();
    this->removeLine();
    this->removeLine();
}

void SimpleLineCollection::removeLine()
{
    SimpleLine* line = m_lineList.last();
    m_lineList.pop_back();
    delete line;
}

void SimpleLineCollection::setLineData(const QList<SimpleLine::LineAttributes> &attributes)
{
    Q_ASSERT(m_lineList.isEmpty() == false);

    QVectorIterator<SimpleLine*> lineIt(m_lineList);

    foreach(SimpleLine::LineAttributes attr, attributes)
    {
        Q_ASSERT(lineIt.hasNext());

        SimpleLine* line = lineIt.next();

//        qDebug() << line;

        line->setLineData(attr);
    }

}
