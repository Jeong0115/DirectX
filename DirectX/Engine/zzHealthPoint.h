#pragma once

#include "zzComponent.h"

namespace zz
{
    class HealthPoint : public Component
    {
    public:
        HealthPoint();
        virtual ~HealthPoint();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void ChangeCurHP(float hp) { mCurHP += hp; }
        void Hit(float damage);
        void AddMaxHP(float add) { mCurHP *= (mMaxHP + add) / mMaxHP; mMaxHP += add; }
        void SetMaxHP(float hp) { mCurHP = mMaxHP = hp; }
        
        float GetCurHP() { return mCurHP; }
        float GetMaxHP() { return mMaxHP; }

        void SetHpZeroEvent(const std::function<void()>& callback) { mDeadEvent = callback; }
        void SetHitEvent(const std::function<void()>& callback) { mHitEvent = callback; }

    private:
        std::function<void()> mDeadEvent;
        std::function<void()> mHitEvent;

        float mMaxHP;
        float mCurHP;

        bool mbDead;
    };
}


