#include "zzElement.h"
#include "zzPixelGrid.h"

namespace zz
{
    Element::Element()
        : mColor(0xFFFF00FF)
        , mType(eElementType::None)
    {
        mStep = PixelGrid::Step;
        mHeatResistance = 100;
    }

    Element::~Element()
    {

    }

    Element::eElementType Element::CheckTargetType(int targetX, int targetY)
    {
        if (targetX < 0 || targetY < 0) return eElementType::Solid;
        Element* target = PixelGrid::GetElement(targetX, targetY);

        if (target == nullptr)
            return eElementType::None;

        return target->GetType();
    }

    void Element::SwapElement(Element* target) // 수정 예정
    { 
        if (target == nullptr) return;

        Position targetPos = target->GetPos();
        if (mPos == targetPos) return;

        this->mStopCount = 0;
        target->mStopCount = 0;

        PixelGrid::SwapElement(mPos.x, mPos.y, targetPos.x, targetPos.y);
    }

    void Element::MoveLastPosition(Position pos)
    {
        if (mPos == pos) return;
        this->mStopCount = 0;

        if(PixelGrid::GetElement(pos.x, pos.y) != nullptr)
            PixelGrid::GetElement(pos.x, pos.y)->mStopCount = 0;

        PixelGrid::SwapElement(mPos.x, mPos.y, pos.x, pos.y);
    }

    void Element::SwapTarget(Position targetPos)
    {
        if (mPos == targetPos) return;
        this->mStopCount = 0;

        if (PixelGrid::GetElement(targetPos.x, targetPos.y) != nullptr)
            PixelGrid::GetElement(targetPos.x, targetPos.y)->mStopCount = 0;

        PixelGrid::SwapElement(mPos.x, mPos.y, targetPos.x, targetPos.y);
    }

    void Element::MoveLastPosAndSwapTarget(Position targetPos, Position lastPos) // 다시 한번 보자
    {
        if (mPos == lastPos || lastPos == targetPos) 
        {
            MoveLastPosition(targetPos);
            return;
        }

        if (mPos == targetPos) 
        {
            MoveLastPosition(lastPos);
            return;
        }

        MoveLastPosition(lastPos);
        SwapTarget(targetPos);
        //matrix.setElementAtIndex(moveToLocationMatrixX, moveToLocationMatrixY, toSwap);
    }

    bool Element::transferHeatToNeighbors()
    {
        //if ((PixelGrid::GetFrameCount() != (UINT)eFrameInfo::Effect) || !shouldApplyHeat()) return false;
        //for (int y = mPos.y - 1; y <= mPos.y + 1; y++)
        //{
        //    for (int x = mPos.x- 1; x <= mPos.x + 1; x++)
        //    {
        //        if (!(x == 0 && y == 0)) 
        //        {
        //            Element* neighbor = PixelGrid::GetElement(x, y);

        //            if (neighbor != nullptr) // 수정예정
        //            {
        //                neighbor->receiveHeat(matrix, heatFactor);
        //            }
        //        }
        //    }
        //}
        return true;
    }

    bool Element::receiveHeat(int heat) 
    {
        if (mIsIgnited) 
            return false;
        
        mHeatResistance -= (int)(random() * heat);
        checkIfIgnited();
        return true;
    }

    bool Element::receiveCooling(int cooling) 
    {
        if (mIsIgnited)
        {
            mHeatResistance += cooling;
            checkIfIgnited();
            return true;
        }
        return false;
    }

    void Element::checkIfIgnited() 
    {
       /* if (mHeatResistance <= 0)
        {
            mIsIgnited = true;
            modifyColor();
        }
        else 
        {
            mIsIgnited = false;
            this.color = ColorConstants.getColorForElementType(elementType, this.getMatrixX(), this.getMatrixY());
        }*/
    }









    //uint32_t Element::rand = 12345;

//#ifdef _DEBUG
//    int Element::a = 0;
//    float Element::sum = 0;
//#endif

    uint32_t Element::xorshift32()
    {
        static uint32_t rand = 12345;
        // A simple xorshift pseudo-random number generator
        rand ^= rand << 13;
        rand ^= rand >> 17;
        rand ^= rand << 5;

//#ifdef _DEBUG
//        a++;
//        sum += rand % 2;
//
//        wchar_t text[50] = {};
//        swprintf_s(text, 50, L"Average rand : %f\n", sum / a);
//
//        OutputDebugStringW(text);
//
//
//#endif
       
        return rand;
    }
}