#pragma once

#include "zzSpell.h"

namespace zz
{
    class ProjectileSpell : public Spell
    {
    public:
        ProjectileSpell();
        ~ProjectileSpell();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual ProjectileSpell* Clone();

        void SetDirection(Vector3 dir) { mDirection = dir; }
        void SetSpeedFactor(float factor) { mSpeed *= factor; }

    protected:
        Vector3 mDirection;

        float   mSpeed;
        float   mSpread;
        float   mDamage;
    };
}   
