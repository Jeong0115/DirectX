#include "zzWater.h"
#include "zzPixelGrid.h"

namespace zz
{
    Water::Water()
    {
        mSpeed = 8;
        mColor = 0xFF0000FF;
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
                //for (int i = 1; i <= mSpeed; i++)
                //{
                //    if (CheckTargetType(mX + i, mY + 1) == eElementType::None)
                //    {
                //        SwapElement(mX + i, mY + 1);
                //        return;
                //    }
                //}
                //for (int i = -1; i >= -mSpeed; i--)
                //{
                //    if (CheckTargetType(mX + i, mY + 1) == eElementType::None)
                //    {
                //        SwapElement(mX + i, mY + 1);
                //        return;
                //    }
                //}
                switch (xorshift32() % 2)
                {
                case 0:
                {
                    for (int i = 1; i <= mSpeed; i++)
                    {
                        if (CheckTargetType(mX + i, mY) == eElementType::None)
                        {
                            SwapElement(mX + i, mY);
                            return;
                        }
                    }
                    break;
                }
                case 1:
                {
                    for (int i = -1; i >= -mSpeed; i--)
                    {
                        if (CheckTargetType(mX + i, mY) == eElementType::None)
                        {
                            SwapElement(mX + i, mY);
                            return;
                        }
                    }
                    break;
                }
                }
                break;

            }

            case 1:
            {
                //for (int i = -1; i >= -mSpeed; i--)
                //{
                //    if (CheckTargetType(mX + i, mY + 1) == eElementType::None)
                //    {
                //        SwapElement(mX + i, mY + 1);
                //        return;
                //    }
                //}
                //for (int i = 1; i <= mSpeed; i++)
                //{
                //    if (CheckTargetType(mX + i, mY + 1) == eElementType::None)
                //    {
                //        SwapElement(mX + i, mY + 1);
                //        return;
                //    }
                //}
                switch (xorshift32() % 2)
                {
                case 0:
                {
                    for (int i = 1; i <= mSpeed; i++)
                    {
                        if (CheckTargetType(mX + i, mY) == eElementType::None)
                        {
                            SwapElement(mX + i, mY);
                            return;
                        }
                    }
                    break;
                }
                case 1:
                {
                    for (int i = -1; i >= -mSpeed; i--)
                    {
                        if (CheckTargetType(mX + i, mY) == eElementType::None)
                        {
                            SwapElement(mX + i, mY);
                            return;
                        }
                    }
                    break;
                }

                }

                break;
            }
            }
        }
    }
    Element* Water::Clone()
    {
        return new Water();
    }
}