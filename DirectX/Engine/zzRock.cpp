#include "zzRock.h"

namespace zz
{
    Rock::Rock()
    {
        mColor = 0xFF808080;
        isFalling = false;
    }
    Rock::~Rock()
    {
    }

    void Rock::Move()
    {
    }

    Element* Rock::Clone()
    {
        return new Rock();
    }
}