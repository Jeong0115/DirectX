#include "zzSand.h"
#include "zzPixelGrid.h"
#include "zzTime.h"

namespace zz
{
    Sand::Sand()
    {
        switch (xorshift32() % 3)
        {
        case 0:
            mColor = 0xFFFFFF00;
            break;

        case 1:
            mColor = 0xFFB2C906;
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


    /*void Sand::Move()
    {
        //if (!isFalling) return;

        PixelGrid& pixelGrid = PixelGrid::GetInst();

        eElementType targetType = CheckTargetType(mX, mY + 1);
        if (targetType == eElementType::Liquid || targetType == eElementType::None)
        {
            SwapElement(mX, mY + 1);
        }
        else if (targetType == eElementType::Solid)
        {
            switch (xorshift32() % 2)
            {
            case 0:
            {
                for (int i = 1; i <= mSpeed; i++)
                {
                    targetType = CheckTargetType(mX + i, mY + 1);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + 1);
                        return;
                    }
                }
                for (int i = -1; i >= -mSpeed; i--)
                {
                    targetType = CheckTargetType(mX + i, mY + 1);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + 1);
                        return;
                    }
                }

            }
            break;

            case 1:
            {
                for (int i = -1; i >= -mSpeed; i--)
                {
                    targetType = CheckTargetType(mX + i, mY + 1);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + 1);
                        return;
                    }
                }
                for (int i = 1; i <= mSpeed; i++)
                {
                    targetType = CheckTargetType(mX + i, mY + 1);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + 1);
                        return;
                    }
                }
            }
            break;

            default:
                break;
            }

        }
    }*/



    void Sand::Move()
    {
       /* y += temp;

        eElementType targetType;

        for(int i = y; i >= 1; i--)
        {
            targetType = CheckTargetType(mX, mY + i);
            if (targetType == eElementType::Liquid || targetType == eElementType::None)
            {
                SwapElement(mX, mY + i);
                y -= i;
                return;
            }
        }

        switch (xorshift32() % 2)
        {
        case 0:
        {
            for (int j = y; j >= 1; j--)
            {
                for (int i = 1; i <= mSpeed; i++)
                {
                    targetType = CheckTargetType(mX + i, mY + j);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + j);
                        y -= j;
                        return;
                    }
                }
            }
            for (int j = y; j >= 1; j--)
            {
                for (int i = -1; i >= -mSpeed; i--)
                {
                    targetType = CheckTargetType(mX + i, mY + j);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + j);
                        y -= j;
                        return;
                    }
                }
            }

        }
        break;

        case 1:
        {
            for (int j = y; j >= 1; j--)
            {
                for (int i = -1; i >= -mSpeed; i--)
                {
                    targetType = CheckTargetType(mX + i, mY + j);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + j);
                        y -= j;
                        return;
                    }
                }
            }
            for (int j = y; j >= 1; j--)
            {
                for (int i = 1; i <= mSpeed; i++)
                {
                    targetType = CheckTargetType(mX + i, mY + j);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + i, mY + j);
                        y -= j;
                        return;
                    }
                }
            }
        }
        break;

        default:
            break;
        }

        y = 0;*/

    }

    Element* Sand::Clone()
    {
        return new Sand();
    }

}