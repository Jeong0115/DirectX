#pragma once

#include "zzEngine.h"
#include "zzPosition.h"

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
        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position& lastPos, int depth) = 0;

        virtual void Move() = 0;
        virtual void Update() = 0;
        eElementType CheckTargetType(int targetX, int targetY);
        void SwapElement(Element* target);
        void SwapLastPosition(Position pos);

        void SetPos(Position pos) { mPos = pos; }
        void SetPos(int x, int y) { mPos = Position(x, y); }
        Position GetPos() { return mPos; }

        uint32_t* GetColor() { return &mColor; }
        void SetColor(uint32_t color) { mColor = color; }

        eElementType GetType() { return mType; }

        math::Vector2 GetVelocity() { return mVelocity; }
        void SetVelocity(math::Vector2 velocity) { mVelocity = velocity; }

        bool isFreeFalling = true;
        bool Is() { return mStopCount >= mStopThreshold; }
        
        __forceinline float GetInertialResistance() { return mInertialResistance; }

        bool IsStop(Position startPos) { return startPos == mPos; }

        bool IsUpdate() {   
            if (isUpdate)
            {
                isUpdate = false;
                return true;
            }
            return false;
        }

        uint32_t xorshift32();
       
        bool isUpdate = false;
//#ifdef _DEBUG
//        static int a;
//        static float sum;
//#endif

    protected:
        math::Vector2 mVelocity;
        math::Vector2 mVelocityRemainder;
        float mInertialResistance = 0.f;

        uint32_t mColor;    
        Position mPos;
        //int mStopUpdateCnt;

        eElementType mType;
        float mFrictionFactor = 1.f;
        int mStopCount = 0;
        int mStopThreshold = 2;
        //friend class Liquid;
    private:
        
    };
}