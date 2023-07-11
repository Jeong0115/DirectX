#include "zzRock.h"

namespace zz
{
    Rock::Rock()
    {
        mColor = 0xFF808080;
        isFreeFalling = false;
    }
    Rock::~Rock()
    {
    }



    Element* Rock::Clone()
    {
        return new Rock();
    }
}