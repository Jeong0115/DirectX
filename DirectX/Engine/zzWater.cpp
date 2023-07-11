#include "zzWater.h"
#include "zzPixelGrid.h"

namespace zz
{
    Water::Water()
    {
        mColor = 0xA0376259;

        mDensity = 5;
        mDispersionRate = 5;
        mFrictionFactor = 1.f;
    }
    Water::~Water()
    {
    }
    

    Element* Water::Clone()
    {
        return new Water();
    }
}