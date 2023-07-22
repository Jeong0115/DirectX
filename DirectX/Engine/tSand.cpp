#include "tSand.h"
#include "zzPixelGrid.h"
#include "zzTime.h"

namespace zz
{
    tSand::tSand()
    {
        mColor = ElementColor::RandomSandColor();
        SetName(L"tSand");
        mVelocity = math::Vector2(random() > 0.5 ? -1.f : 1.f, 124.f);
        mFrictionFactor = 0.9f;
        mInertialResistance = .1f;
        //mass = 150;
    }

    tSand::~tSand()
    {

    }








    Element* tSand::Clone()
    {
        return new tSand();
    }

    bool tSand::ReceiveHeat(int heat)
    {
        return false;
    }

}