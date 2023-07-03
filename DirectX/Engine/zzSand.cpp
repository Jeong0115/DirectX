#include "zzSand.h"
#include "zzPixelGrid.h"
#include "zzTime.h"

namespace zz
{
    Sand::Sand()
    {
        mColor = 0xFFFFFF00;
    }

    Sand::~Sand()
    {

    }

    //uint32_t xorshift32()
    //{
    //    // A simple xorshift pseudo-random number generator
    //    uint32_t state;
    //    state ^= state << 13;
    //    state ^= state >> 17;
    //    state ^= state << 5;
    //    return state;
    //}

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
    

    void Sand::Move()
    {
        if (!isFalling) return;

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
                targetType = CheckTargetType(mX + 1, mY + 1);
                if (targetType == eElementType::Liquid || targetType == eElementType::None)
                {
                    SwapElement(mX + 1, mY + 1);
                }

                else if (targetType == eElementType::Solid)
                {
                    targetType = CheckTargetType(mX - 1, mY + 1);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX - 1, mY + 1);
                    }
                    else if (targetType == eElementType::Solid)
                    {
                        isFalling = true;
                        pixelGrid.SetActiveChunk(mX, mY );
                    }
                }
            }
            break;

            case 1:
            {
                targetType = CheckTargetType(mX - 1, mY + 1);
                if (targetType == eElementType::Liquid || targetType == eElementType::None)
                {
                    SwapElement(mX - 1, mY + 1);
                }

                else if (targetType == eElementType::Solid)
                {
                    targetType = CheckTargetType(mX + 1, mY + 1);
                    if (targetType == eElementType::Liquid || targetType == eElementType::None)
                    {
                        SwapElement(mX + 1, mY + 1);
                    }
                    else if (targetType == eElementType::Solid)
                    {
                        isFalling = true;
                        pixelGrid.SetActiveChunk(mX, mY);
                    }
                }
            }
            break;

            default:
                break;
            }
           
        } 

        /*eElementType targetType = target->GetType();

        if (targetType == eElementType::Liquid)
        {
            pixelGrid.SwapElement(mX, mY, mX, mY + 1);
            mX = mX;
            mY = mY + 1;
            pixelGrid.SetActiveChunk(mX, mY);
            return;
        }
        else if (targetType == eElementType::Solid)
        {
            Element* targetLeft = pixelGrid.GetElement(mX - 1, mY + 1);

            if (targetLeft == nullptr)
            {
                pixelGrid.SwapElement(mX, mY, mX - 1, mY + 1);
                mX = mX - 1;
                mY = mY + 1;
                pixelGrid.SetActiveChunk(mX, mY);
                return;
            }
            else if (targetLeft->GetType() == eElementType::Liquid)
            {
                pixelGrid.SwapElement(mX, mY, mX - 1, mY + 1);
                mX = mX - 1;
                mY = mY + 1;
                pixelGrid.SetActiveChunk(mX, mY);
                return;
            }
            else if (targetLeft->GetType() == eElementType::Solid)
            {
                Element* targetRight = pixelGrid.GetElement(mX + 1, mY + 1);

                if (targetRight == nullptr)
                {
                    pixelGrid.SwapElement(mX , mY, mX + 1, mY + 1);
                    mX = mX + 1;
                    mY = mY + 1;
                    pixelGrid.SetActiveChunk(mX, mY);
                    return;
                }
                else if (targetRight->GetType() == eElementType::Liquid)
                {
                    pixelGrid.SwapElement(mX, mY, mX + 1, mY + 1);
                    mX = mX + 1;
                    mY = mY + 1;
                    pixelGrid.SetActiveChunk(mX, mY);
                    return;
                }
                else
                {
                    isFalling = false;
                    return;
                }
            }
        }*/
    }

}