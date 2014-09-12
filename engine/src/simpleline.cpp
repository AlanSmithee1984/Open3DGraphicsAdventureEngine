#include "simpleline.h"

#include <OgreSceneManager.h>
#include <OgreManualObject.h>


#include <QtDebug>

SimpleLine::SimpleLine(Ogre::SceneManager *manager)
    : m_sceneManager(manager),
      m_manual(NULL),
      m_sceneNode(NULL)
{
    static quint32 counter = 0;

    Ogre::String name = "SimpleLine_" + Ogre::StringConverter::toString(++counter);

    m_manual = m_sceneManager->createManualObject(name);


    this->createGeometry();
}

void SimpleLine::setLineData(const LineAttributes &attr)
{

}

void SimpleLine::createGeometry()
{
    Q_ASSERT(m_manual);

    m_manual->begin("SimpleLine");

    const Ogre::Real largeNumber = 10E+10;

    const Ogre::Vector3 min(-largeNumber, -largeNumber, -largeNumber);
    const Ogre::Vector3 max(largeNumber, largeNumber, largeNumber);

    m_manual->position(min);
    m_manual->textureCoord(0, 0);

    m_manual->position(min);
    m_manual->textureCoord(1, 0);

    m_manual->position(max);
    m_manual->textureCoord(1, 1);



    m_manual->position(min);
    m_manual->textureCoord(0, 0);

    m_manual->position(min);
    m_manual->textureCoord(1, 1);

    m_manual->position(max);
    m_manual->textureCoord(0, 1);

    m_manual->end();

    Q_ASSERT(m_sceneNode == NULL);
    m_sceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_sceneNode->attachObject(m_manual);
}


SimpleLine::LineAttributes::LineAttributes(const Ogre::Vector3 start, const Ogre::Vector3 end,
                                           const Ogre::ColourValue &colorStart, const Ogre::ColourValue &colorEnd)
    : startPos(start),
      endPos(end),
      vertexColorStart(colorStart),
      vertexColorEnd(colorEnd)
{

}