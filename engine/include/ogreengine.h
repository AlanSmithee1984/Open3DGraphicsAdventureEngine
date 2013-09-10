#ifndef OGREENGINE_H
#define OGREENGINE_H

#include "singleton.h"

class OgreEngine : public Singleton<OgreEngine>
{
public:
    void initialize();

private:
    friend class Singleton<OgreEngine>;
    OgreEngine();

};

#endif // OGREENGINE_H
