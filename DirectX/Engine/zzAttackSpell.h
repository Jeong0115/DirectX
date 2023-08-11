#pragma once

#include "zzSpell.h"

namespace zz
{
    class AttackSpell : public Spell
    {
    public:
        AttackSpell();
        ~AttackSpell();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        void SetDirection(Vector3 dir) { mDirection = dir; }

    protected:
        Vector3 mDirection;
    };
}
