#ifndef SIMPLELINE_H
#define SIMPLELINE_H

#include <OgreMaterial.h>
#include <OgreVector3.h>

namespace Ogre
{
class ManualObject;
class SceneManager;
}

class SimpleLine
{
public:

    struct LineAttributes
    {
        LineAttributes(const Ogre::Vector3 start,
                       const Ogre::Vector3 end,
                       const Ogre::ColourValue &colorStart,
                       const Ogre::ColourValue &colorEnd);

        Ogre::Vector3 startPos;
        Ogre::Vector3 endPos;
        Ogre::ColourValue vertexColorStart;
        Ogre::ColourValue vertexColorEnd;
    };

    SimpleLine(Ogre::SceneManager* manager);
    ~SimpleLine();

    void setLineData(const LineAttributes &attr);

private:
    void createGeometry();

    Ogre::SceneManager* m_sceneManager;
    Ogre::ManualObject* m_manual;
    Ogre::SceneNode* m_sceneNode;
    Ogre::MaterialPtr m_material;
};

#endif // SIMPLELINE_H
