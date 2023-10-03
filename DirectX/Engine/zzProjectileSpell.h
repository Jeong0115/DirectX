#pragma once

#include "zzSpell.h"
#include "zzMuzzleEffect.h"

namespace zz
{
    class ProjectileSpell : public Spell
    {
    public:
        ProjectileSpell();
        virtual ~ProjectileSpell();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual ProjectileSpell* Clone();

        GameObject* GetMuzzleEffect() { return mMuzzleEffect; }

        void SetDirection(Vector3 dir) { mDirection = dir; }
        void SetSpeedFactor(float factor) { mSpeed *= factor; }

    protected:
        MuzzleEffect* mMuzzleEffect;

        Vector3     mDirection;

        float   mSpeed;
        float   mSpread;
        float   mDamage;
    };
}   
