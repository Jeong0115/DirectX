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
        void AddMaxHP(float add) { mCurHP *= (mMaxHP + add) / mMaxHP; mMaxHP += add; }
        void SetMaxHP(float hp) { mCurHP = mMaxHP = hp; }

        float GetCurHP() { return mCurHP; }

    private:
        float mMaxHP;
        float mCurHP;
    };
}

