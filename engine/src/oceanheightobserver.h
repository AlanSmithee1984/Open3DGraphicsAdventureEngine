#ifndef OCEANHEIGHTOBSERVER_H
#define OCEANHEIGHTOBSERVER_H

namespace Hydrax
{
    class Hydrax;
}

class OceanHeightObserver
{
public:
    OceanHeightObserver();
    virtual ~OceanHeightObserver();

    virtual void oceanHeightUpdated(Hydrax::Hydrax* hydrax) = 0;
};

#endif // OCEANHEIGHTOBSERVER_H
