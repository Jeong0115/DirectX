#include "zzOil.h"

namespace zz
{
    Oil::Oil()
    {
        mColor = 0xe63D3728;

        mDensity = 4;
        mDispersionRate = 4;
        mFrictionFactor = 1.f;

    }
    Oil::~Oil()
    {
    }
    Element* Oil::Clone()
    {
        return new Oil();
    }
}