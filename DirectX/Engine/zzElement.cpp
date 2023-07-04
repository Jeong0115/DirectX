#include "zzElement.h"
#include "zzPixelGrid.h"

namespace zz
{
    Element::Element()
    {

    }

    Element::~Element()
    {

    }

    Element::eElementType Element::CheckTargetType(int targetX, int targetY)
    {
        PixelGrid& pixelGrid = PixelGrid::GetInst();
        if (targetX < 0 || targetY < 0) return eElementType::Solid;
        Element* target = pixelGrid.GetElement(targetX, targetY);

        if (target == nullptr)
            return eElementType::None;

        return target->GetType();
    }

    void Element::SwapElement(int destX, int destY)
    {
        PixelGrid& pixelGrid = PixelGrid::GetInst();

        if (mX % 64 == 0)
        {
            pixelGrid.SetActiveChunk(mX - 1, mY);
        }
        if (mX % 64 == 63)
        {
            pixelGrid.SetActiveChunk(mX + 1, mY);
        }
        if (mY % 64 == 0)
        {
            pixelGrid.SetActiveChunk(mX, mY - 1);
        }
        if (mY % 64 == 63)
        {
            pixelGrid.SetActiveChunk(mX, mY + 1);
        }

        pixelGrid.SwapElement(mX, mY, destX, destY);
        //mX = destX;
        //mY = destY;
        pixelGrid.SetActiveChunk(mX, mY);

        

        
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