#include "simplelinecollection.h"

#include <QDebug>

SimpleLineCollection::SimpleLineCollection(Ogre::SceneManager* sceneManager)
    : m_sceneManager(sceneManager)
{
}

void SimpleLineCollection::addLine()
{
    SimpleLine* line = new SimpleLine(m_sceneManager);

    m_lineList << line;
}

void SimpleLineCollection::removeLine()
{
    SimpleLine* line = m_lineList.last();
    m_lineList.pop_back();
    delete line;
}

void SimpleLineCollection::setLineData(const QList<SimpleLine::LineAttributes> &attributes)
{
    this->adjustLineNumber(attributes.size());

    QVectorIterator<SimpleLine*> lineIt(m_lineList);

    foreach(SimpleLine::LineAttributes attr, attributes)
    {
        Q_ASSERT(lineIt.hasNext());

        SimpleLine* line = lineIt.next();

//        qDebug() << line;

        line->setLineData(attr);
    }

}

void SimpleLineCollection::adjustLineNumber(quint32 numberOfDesiredLines)
{
    qint32 diff = m_lineList.size() - numberOfDesiredLines;

    if(diff > 0)
    {
        // reduce
        for(quint32 i = 0; i < diff ; ++i)
        {
            this->removeLine();
        }
    }
    else if(diff < 0)
    {
        // increase
        for(quint32 i = 0; i < (-diff) ; ++i)
        {
            this->addLine();
        }
    }
    else
    {
        // do nothing
        Q_ASSERT(diff == 0);
    }
}
