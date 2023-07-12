#include "zzOil.h"

namespace zz
{
    Oil::Oil()
    {
        mColor = ElementColor::Oil;

        mDensity = 4;
        mDispersionRate = 4;
        mFrictionFactor = 1.f;
        mInertialResistance = 0;
        mVelocity = math::Vector2(0, 124.f);

    }
    Oil::~Oil()
    {
    }
    Element* Oil::Clone()
    {
        return new Oil();
    }
}