#include "zzWater.h"
#include "zzPixelGrid.h"
#include "zzElementColor.h"
namespace zz
{
    Water::Water()
    {
        mColor = ElementColor::Water;

        mDensity = 5;
        mDispersionRate = 5;
        mFrictionFactor = 1.f;
        mInertialResistance = 0;
        mVelocity = math::Vector2(0, 124.f);
    }
    Water::~Water()
    {
    }
    

    Element* Water::Clone()
    {
        return new Water();
    }
}