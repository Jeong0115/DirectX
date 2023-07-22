#pragma once

#include "zzEngine.h"
#include "zzPosition.h"
#include "zzElementColor.h"
#include "zzEntity.h"

namespace zz
{
    enum class eElementType { Solid, Liquid, Gas, Particle, Empty, Out };
    class Element : public Entity
    {
    public:
        
        enum class eFrameInfo { Effect = 1, Reaction = 3 };

        Element();
        virtual ~Element();

        virtual void Update() {};
        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth) { return false; }
        virtual Element* Clone() { return nullptr; }
        
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

        eElementType GetType() const { return mType; }

        math::Vector2 GetVelocity() { return mVelocity; }
        void SetVelocity(math::Vector2 velocity) { mVelocity = velocity; }

        
        bool Is() { return mStopCount >= mStopThreshold; }
        bool IsFreeFalling() { return mbFreeFalling; }
        void SetFreeFalling(bool fall) { mbFreeFalling = fall; }
        
        float GetInertialResistance() { return mInertialResistance; }
        float GetFrictionFactor() { return mFrictionFactor; }

        bool IsStop(Position startPos) { return startPos == mPos; }
        void SetDead() { die(); }
        virtual bool ReceiveHeat(int heat);
        uint32_t xorshift32();
       
        bool isUpdate = false;
        ElementColor mColor;

    protected:
        bool transferHeatToNeighbors();
        bool shouldApplyHeat() { return mbIgnited || mbHeated; }
        
        bool receiveCooling(int cooling);
        void checkIfIgnited();
        void takeEffectsDamage();
        void takeFireDamage();
        void checkIfDead();
        void die();
        void spawnSparkIfIgnited();
        void checkIgnitedAndSetColor();

        bool mbFreeFalling = true;

        math::Vector2 mVelocity;
        math::Vector2 mVelocityRemainder;
        float mInertialResistance = 0.f;

       
        Position mPos;
        //int mStopUpdateCnt;
        std::bitset<1> mStep;

        eElementType mType;
        float mFrictionFactor = 1.f;
        int mStopCount = 0;
        int mStopThreshold = 2;
        bool mbIgnited;
        int mHeatResistance;
        int mHeatFactor;
        int mFireDamage;
        int mHealth;
    private:
        bool isSurrounded();

    private:
        
        bool mbHeated;
        
        
        
        int resetFlammabilityResistance;
    };
}