#ifndef VERTEXINFORMATION_H
#define VERTEXINFORMATION_H

#include <OgreVector3.h>
#include <QList>

class VertexInformation
{
public:
    static VertexInformation *construct(const Ogre::Vector3 &center, const Ogre::Vector3 &rightOf, const Ogre::Vector3 &leftOf);


    VertexInformation(const Ogre::Vector3 position,
                      const Ogre::Vector3 edgeP,
                      const Ogre::Vector3 edgeQ);

    Ogre::Real calcVertexVolumeEquation() const;

private:
    const Ogre::Vector3 m_position;
    const Ogre::Vector3 m_edgeP;
    const Ogre::Vector3 m_edgeQ;


};

typedef QList<VertexInformation*> VertexInformations;

#endif // VERTEXINFORMATION_H
