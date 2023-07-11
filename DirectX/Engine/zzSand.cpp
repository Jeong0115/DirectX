#include "zzSand.h"
#include "zzPixelGrid.h"
#include "zzTime.h"

namespace zz
{
    Sand::Sand()
    {
        switch (xorshift32() % 1)
        {
        case 0:
            mColor = 0xffb89e57;
            break;

        case 1:
            mColor = 0xff524f2d;
            break;

        case 2:
            mColor = 0xFFE9FC5A;
            break;
        }
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

}