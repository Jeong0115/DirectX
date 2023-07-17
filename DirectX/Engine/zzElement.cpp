#include "zzElement.h"
#include "zzPixelGrid.h"
#include "zzSpark.h"
#include "zzSmoke.h"

namespace zz
{
    Element::Element()
        : mColor(0xFFFF00FF)
        , mType(eElementType::Empty)
        , mbIgnited(false)
        , mbHeated(false)
        , mHeatFactor(10)
        , mFireDamage(3)
        , mHealth(500)
        , resetFlammabilityResistance(mHeatFactor / 2)
    {     
        mStep = PixelGrid::Step;
        mHeatResistance = 100;
    }

    Element::~Element()
    {

    }

    eElementType Element::CheckTargetType(int targetX, int targetY)
    {
        Element* target = PixelGrid::GetElement(targetX, targetY);

        if (target == nullptr)
        {
            return eElementType::Out;
        }

        return target->GetType();
    }

    void Element::SwapElement(Element* target)
    { 
        if (target == nullptr) return;
        if (this == target) return; // 체크

        Position targetPos = target->GetPos();

        this->mStopCount = 0;
        target->mStopCount = 0;

        PixelGrid::SwapElement(mPos.x, mPos.y, targetPos.x, targetPos.y);
    }

    void Element::MoveLastPosition(Position pos)
    {
        if (mPos == pos) return;

        this->mStopCount = 0;

        if(PixelGrid::GetElement(pos.x, pos.y) != nullptr)
        {
            PixelGrid::GetElement(pos.x, pos.y)->mStopCount = 0;
        }

        PixelGrid::SwapElement(mPos.x, mPos.y, pos.x, pos.y);
    }

    void Element::SwapTarget(Position targetPos)
    {
        if (mPos == targetPos) return;

        this->mStopCount = 0;

        if (PixelGrid::GetElement(targetPos.x, targetPos.y) != nullptr)
        {
            PixelGrid::GetElement(targetPos.x, targetPos.y)->mStopCount = 0;
        }

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
        if ((PixelGrid::GetFrameCount() != (UINT)eFrameInfo::Effect) || !shouldApplyHeat()) return false;
        for (int y = mPos.y - 1; y <= mPos.y + 1; y++)
        {
            for (int x = mPos.x- 1; x <= mPos.x + 1; x++)
            {
                if (!(x == 0 && y == 0)) 
                {
                    Element* neighbor = PixelGrid::GetElement(x, y);

                    
                    if (neighbor != nullptr) 
                    {
                        if (neighbor->GetType() == eElementType::Empty) continue; // 수정해야딤
                        neighbor->ReceiveHeat(mHeatFactor);
                    }
                }
            }
        }
        return true;
    }

    bool Element::ReceiveHeat(int heat) 
    {
        if (mbIgnited) 
        {
            return false;
        }
        
        mHeatResistance -= (int)(random() * heat);
        checkIfIgnited();
        return true;
    }

    bool Element::receiveCooling(int cooling) 
    {
        if (mbIgnited)
        {
            mHeatResistance += cooling;
            checkIfIgnited();
            return true;
        }
        return false;
    }

    void Element::checkIfIgnited() 
    {
        if (mHeatResistance <= 0)
        {
            mbIgnited = true;
            checkIgnitedAndSetColor();
        }
        else 
        {
            mbIgnited = false;
            // 나중에 수정해야됨 
            PixelGrid::SetPixelColor(mPos.x, mPos.y, mColor.color);
        }
    }

    void Element::takeEffectsDamage() 
    {
        if (PixelGrid::GetFrameCount() != (UINT)eFrameInfo::Effect) 
        {
            return;
        }

        if (mbIgnited) {
            takeFireDamage();
        }
        checkIfDead();
    }

    void Element::takeFireDamage()
    {
        mHealth -= mFireDamage;
        if (isSurrounded()) 
        {
            mHeatResistance = resetFlammabilityResistance;
        }
        checkIfIgnited();
    }

    void Element::checkIfDead()
    {
        if (mHealth <= 0) 
        {
            die();
        }
    }

    void Element::die() 
    {
        PixelGrid::DeleteElement(mPos.x, mPos.y);
        PixelGrid::SetActiveChunks(mPos.x, mPos.y);
        //delete this;
    }

    void Element::spawnSparkIfIgnited()
    {
        if ((PixelGrid::GetFrameCount() != (UINT)eFrameInfo::Effect) || !mbIgnited) 
        {
            return;
        }
        Element* upElement = PixelGrid::GetElement(mPos.x, mPos.y);

        if (upElement == nullptr) return;

        if (upElement->GetType() == eElementType::Empty)
        {

            random() > 0.1f ? PixelGrid::CreateElement(mPos.x, mPos.y, new Spark()) 
                : PixelGrid::CreateElement(mPos.x, mPos.y, new Smoke());
        }
    }

    void Element::checkIgnitedAndSetColor()
    {
        if(mbIgnited)
        {
            PixelGrid::SetPixelColor(mPos.x, mPos.y, ElementColor::RandomFireColor().color);
        }
    }


    bool Element::isSurrounded() 
    {
        if (PixelGrid::GetElement(mPos.x - 1, mPos.y)->GetType() == eElementType::Empty
            || PixelGrid::GetElement(mPos.x + 1, mPos.y)->GetType() == eElementType::Empty
            || PixelGrid::GetElement(mPos.x, mPos.y - 1)->GetType() == eElementType::Empty
            || PixelGrid::GetElement(mPos.x, mPos.y + 1)->GetType() == eElementType::Empty)
        {
            return false;
        }

        return true;
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