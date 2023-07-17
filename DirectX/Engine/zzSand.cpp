#include "zzSand.h"
#include "zzPixelGrid.h"
#include "zzTime.h"

namespace zz
{
    Sand::Sand()
    {
        mColor = ElementColor::RandomSandColor();

        mVelocity = math::Vector2(random() > 0.5 ? -1.f : 1.f, 124.f);
        mFrictionFactor = 0.9f;
        mInertialResistance = .1f;
        //mass = 150;
    }

    Sand::~Sand()
    {

    }



    uint32_t xorshift32()
    {
        // A simple xorshift pseudo-random number generator
        uint32_t state;
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    uint32_t x = 3, y = 1563, z, w;

    uint32_t xorshift128(void) {
        uint32_t t = x;
        t ^= t << 11;
        t ^= t >> 8;
        x = y; y = z; z = w;
        w ^= w >> 19;
        w ^= t;
        return w;
    }






    Element* Sand::Clone()
    {
        return new Sand();
    }

    bool Sand::ReceiveHeat(int heat)
    {
        return false;
    }

}