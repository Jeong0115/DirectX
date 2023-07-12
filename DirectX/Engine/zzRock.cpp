#include "zzRock.h"

namespace zz
{
    Rock::Rock()
    {
        mColor = ElementColor::Rock;
       
        mVelocity = math::Vector2(0.f, 0.f);
        mFrictionFactor = 0.5f;
        mInertialResistance = 1.1f;
    }
    Rock::~Rock()
    {
    }



    Element* Rock::Clone()
    {
        return new Rock();
    }
}