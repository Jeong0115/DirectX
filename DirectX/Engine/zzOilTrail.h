#pragma once

#include "zzModifierSpell.h"

namespace zz
{
    class ProjectileSpell;
    class OilTrail : public ModifierSpell
    {
    public:
        OilTrail();
        virtual ~OilTrail();

        virtual ProjectileSpell* ModifierProjectile(ProjectileSpell* projectile) override;
    };
}

