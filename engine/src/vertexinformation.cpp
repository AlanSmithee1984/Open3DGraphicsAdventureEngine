#include "vertexinformation.h"

VertexInformation* VertexInformation::construct(const Ogre::Vector3 &center,
                                                const Ogre::Vector3 &rightOf,
                                                const Ogre::Vector3 &leftOf)
{
    return new VertexInformation(center,
                                 (center - rightOf).normalisedCopy(),
                                 (center - leftOf).normalisedCopy()
                                 );

}

VertexInformation::VertexInformation()
{

}

VertexInformation::VertexInformation(const Ogre::Vector3 &position,
                                     const Ogre::Vector3 &edgeP,
                                     const Ogre::Vector3 &edgeQ)
    : m_position(position),
      m_edgeP(edgeP),
      m_edgeQ(edgeQ)
{

}

void VertexInformation::setInfo(const Ogre::Vector3 &position,
                                const Ogre::Vector3 &edgeP,
                                const Ogre::Vector3 &edgeQ)
{
    m_position = position;
    m_edgeP = edgeP;
    m_edgeQ = edgeQ;
}

Ogre::Real VertexInformation::calcVertexVolumeEquation() const
{
    const Ogre::Real vDotP = m_position.dotProduct(m_edgeP);
    const Ogre::Real vDotQ = m_position.dotProduct(m_edgeQ);
    const Ogre::Real inner = vDotP * vDotP + vDotQ * vDotQ;

    const Ogre::Real pDotQ = m_edgeP.dotProduct(m_edgeQ);
    const Ogre::Real subtrahend = pDotQ * inner;

    const Ogre::Real minuend = 2.0f * vDotP * vDotQ;

    const Ogre::Real rightBracked = minuend - subtrahend;



    const Ogre::Vector3 pCrossQ = m_edgeP.crossProduct(m_edgeQ);
    const Ogre::Real upper = m_position.dotProduct(pCrossQ);

    const Ogre::Real lower = 1.0f - pDotQ * pDotQ;

    const Ogre::Real div = upper / lower;

    return div * rightBracked / 6.0f;


}

