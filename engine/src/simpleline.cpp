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
    Q_ASSERT(m_material.isNull() == false);
    Ogre::GpuProgramParametersSharedPtr params = m_material->getTechnique(0)->getPass(0)->getVertexProgramParameters();

//    std::cerr << this << "\t" << m_material.get() << "\t"
//              << m_material->getName() << "\t" << params.get() << "\t"
//              << attr.startPos << "\t" << attr.endPos << "\t"
//              << attr.vertexColorStart << "\t" << attr.vertexColorEnd << std::endl;

    params->setNamedConstant("start", attr.startPos);
    params->setNamedConstant("end", attr.endPos);

    Ogre::Vector4 startCol(attr.vertexColorStart.r, attr.vertexColorStart.g,
                           attr.vertexColorStart.b, attr.vertexColorStart.a);
    params->setNamedConstant("startCol", startCol);

    Ogre::Vector4 endCol(attr.vertexColorEnd.r, attr.vertexColorEnd.g,
                         attr.vertexColorEnd.b, attr.vertexColorEnd.a);
    params->setNamedConstant("endCol", endCol);
}

void SimpleLine::createGeometry()
{
    static quint32 counter = 0;

    Ogre::String matName = "SimpleLineMat_" + Ogre::StringConverter::toString(++counter);


    m_material = Ogre::MaterialManager::getSingletonPtr()->getByName("simpleline")->clone(matName);

    Q_ASSERT(m_manual);
    m_manual->begin(m_material->getName());

    const Ogre::Real largeNumber = 10E+10;

    const Ogre::Vector3 min(-largeNumber, -largeNumber, -largeNumber);
    const Ogre::Vector3 max(largeNumber, largeNumber, largeNumber);

    m_manual->position(min);
    m_manual->textureCoord(0, 0);

    m_manual->position(min);
    m_manual->textureCoord(1, 0);

    m_manual->position(max);
    m_manual->textureCoord(1, 1);

    m_manual->position(max);
    m_manual->textureCoord(0, 1);

    m_manual->quad(0, 1, 2, 3);

    m_manual->end();

    Q_ASSERT(m_sceneNode == NULL);
    m_sceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_sceneNode->attachObject(m_manual);

    m_sceneNode->showBoundingBox(true);
}


SimpleLine::LineAttributes::LineAttributes(const Ogre::Vector3 start, const Ogre::Vector3 end,
                                           const Ogre::ColourValue &colorStart, const Ogre::ColourValue &colorEnd)
    : startPos(start),
      endPos(end),
      vertexColorStart(colorStart),
      vertexColorEnd(colorEnd)
{

}
