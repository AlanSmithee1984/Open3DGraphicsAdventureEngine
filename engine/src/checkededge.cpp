#include "checkededge.h"

CheckedEdge::CheckedEdge()
{
        this->reset();
}

void CheckedEdge::reset()
{
    start.first = false;
    end.first = false;
}

