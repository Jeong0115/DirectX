#include "zzWater.h"
#include "zzPixelGrid.h"

namespace zz
{
    Water::Water()
    {
        mSpeed = 5;
        mColor = 0xA0376259;
    }
    Water::~Water()
    {
    }
    void Water::Move()
    {
        //if (!isFalling) return;

        PixelGrid& pixelGrid = PixelGrid::GetInst();

        eElementType targetType = CheckTargetType(mX, mY + 1);
        if (targetType == eElementType::None)
        {
            SwapElement(mX, mY + 1);
        }
        else if (targetType == eElementType::Liquid || targetType == eElementType::Solid)
        {

            switch (xorshift32() % 2)
            {
            case 0:
            {
                //switch (xorshift32() % 2)
                //{
                //case 0:
                //{
                    if(CheckTargetType(mX + 1, mY) == eElementType::None)
                    {
                        for (int i = 2; i <= mSpeed; i++)
                        {
                            if (CheckTargetType(mX + i, mY) != eElementType::None)
                            {
                                SwapElement(mX + i - 1, mY);
                                return;
                            }
                        }
                        SwapElement(mX + mSpeed, mY);
                        return;
                    }
                    //break;
                //}
                //case 1:
                //{
                    if (CheckTargetType(mX - 1, mY) == eElementType::None)
                    {
                        for (int i = 2; i <= mSpeed; i++)
                        {
                            if (CheckTargetType(mX - i, mY) != eElementType::None)
                            {
                                SwapElement(mX - i + 1, mY);
                                return;
                            }
                        }
                        SwapElement(mX - mSpeed, mY);
                        return;
                    }
                    break;
                //}
                //}
                //break;

            }

            case 1:
            {
                //switch (xorshift32() % 2)
                //{
                //case 0:
                //{
                    if (CheckTargetType(mX - 1, mY) == eElementType::None)
                    {
                        for (int i = 2; i <= mSpeed; i++)
                        {
                            if (CheckTargetType(mX - i, mY) != eElementType::None)
                            {
                                SwapElement(mX - i + 1, mY);
                                return;
                            }
                        }
                        SwapElement(mX - mSpeed, mY);
                        return;
                    }
                    //break;
                //}
                //case 1:
                //{
                    if (CheckTargetType(mX + 1, mY) == eElementType::None)
                    {
                        for (int i = 2; i <= mSpeed; i++)
                        {
                            if (CheckTargetType(mX + i, mY) != eElementType::None)
                            {
                                SwapElement(mX + i - 1, mY);
                                return;
                            }
                        }
                        SwapElement(mX + mSpeed, mY);
                        return;
                    }
                    break;
                //}

                //}

                //break;
            }
            }
        }
    }
    Element* Water::Clone()
    {
        return new Water();
    }
}