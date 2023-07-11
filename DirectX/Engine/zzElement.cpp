#include "zzElement.h"
#include "zzPixelGrid.h"

namespace zz
{
    Element::Element()
        : mColor(0xFFFF00FF)
        , mType(eElementType::None)
    {
        mStep = PixelGrid::Step;
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
        Position targetPos = target->GetPos();
        if (mPos.x % 64 == 0)
            PixelGrid::SetActiveChunks(mPos.x - 1, mPos.y);
        else if (mPos.x % 64 == 63)
            PixelGrid::SetActiveChunks(mPos.x + 1, mPos.y);

        if (mPos.y % 64 == 0)
            PixelGrid::SetActiveChunks(mPos.x, mPos.y - 1);
        else if (mPos.y % 64 == 63)
            PixelGrid::SetActiveChunks(mPos.x, mPos.y + 1);

        PixelGrid::SwapElement(mPos.x, mPos.y, targetPos.x, targetPos.y);
        target->mStopCount = 0;
        //PixelGrid::SetActiveChunk(mPos.x, mPos.y);
    }

    void Element::SwapLastPosition(Position pos)
    {
        if (pos.x < 0 || pos.y < 0) return;
        if (mPos == pos) return;
        this->mStopCount = 0;

        if(PixelGrid::GetElement(pos.x, pos.y) != nullptr)
            PixelGrid::GetElement(pos.x, pos.y)->mStopCount = 0;

        PixelGrid::SwapElement(mPos.x, mPos.y, pos.x, pos.y);
        

        //PixelGrid::SetActiveChunk(mPos.x, mPos.y);
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