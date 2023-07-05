#pragma once

#include "zzEngine.h"

namespace zz
{
    class Element
    {
    public:
        enum class eElementType
        {
            Solid,
            Liquid,
            Gas,
            None
        };


        Element();
        virtual ~Element();

        virtual Element* Clone() = 0;

        virtual void Move() = 0;
        eElementType CheckTargetType(int targetX, int targetY);
        void SwapElement(int destX, int destY);

        void SetPos(int x, int y) { mX = x, mY = y; }
        uint32_t* GetColor() { return &mColor; }
        void SetColor(uint32_t color) { mColor = color; }

        eElementType GetType() { return mType; }

        bool isFalling = true;
        

        uint32_t xorshift32();
        //static uint32_t rand;

        bool IsUpdate() {
            if (isUpdate)
            {
                isUpdate = false;
                return true;
            }
            return false;
        }
//#ifdef _DEBUG
//        static int a;
//        static float sum;
//#endif

    protected:
        uint32_t mColor;
        int mX;
        int mY;


        eElementType mType;

    private:
        bool isUpdate = false;
    };
}