#pragma once

#include "zzEngine.h"
#include "zzPosition.h"
#include "zzElementColor.h"

namespace zz
{
    enum class eElementType { Solid, Liquid, Gas, Empty, Out };
    class Element
    {
    public:
        
        enum class eFrameInfo { Effect = 1, Reaction = 3 };

        Element();
        virtual ~Element();

        virtual Element* Clone() = 0;
        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth) = 0;

        virtual void Update() = 0;
        eElementType CheckTargetType(int targetX, int targetY);
        void SwapElement(Element* target);
        void MoveLastPosition(Position pos);
        void SwapTarget(Position targetPos);
        void MoveLastPosAndSwapTarget(Position targetPos, Position lastPos);

        void SetPos(Position pos) { mPos = pos; }
        void SetPos(int x, int y) { mPos = Position(x, y); }
        Position GetPos() { return mPos; }

        uint32_t* GetColor() { return &mColor.color; }
        void SetColor(uint32_t color) { mColor.color = color; }

        eElementType GetType() { return mType; }

        math::Vector2 GetVelocity() { return mVelocity; }
        void SetVelocity(math::Vector2 velocity) { mVelocity = velocity; }

        bool isFreeFalling = true;
        bool Is() { return mStopCount >= mStopThreshold; }
        
        float GetInertialResistance() { return mInertialResistance; }
        float GetFrictionFactor() { return mFrictionFactor; }

        bool IsStop(Position startPos) { return startPos == mPos; }

        uint32_t xorshift32();
       
        bool isUpdate = false;
//#ifdef _DEBUG
//        static int a;
//        static float sum;
//#endif

    protected:
        bool transferHeatToNeighbors();
        bool shouldApplyHeat() { return mbIgnited || mbHeated; }
        bool receiveHeat(int heat);
        bool receiveCooling(int cooling);
        void checkIfIgnited();
        void takeEffectsDamage();
        void takeFireDamage();
        void checkIfDead();
        void die();
        void spawnSparkIfIgnited();


        math::Vector2 mVelocity;
        math::Vector2 mVelocityRemainder;
        float mInertialResistance = 0.f;

        ElementColor mColor;
        Position mPos;
        //int mStopUpdateCnt;
        std::bitset<1> mStep;

        eElementType mType;
        float mFrictionFactor = 1.f;
        int mStopCount = 0;
        int mStopThreshold = 2;

    private:
        bool isSurrounded();

    private:
        bool mbIgnited;
        bool mbHeated;
        int mHeatResistance;
        int mHeatFactor ;
        int mFireDamage ;
        int mHealth ;
        int resetFlammabilityResistance;
    };
}