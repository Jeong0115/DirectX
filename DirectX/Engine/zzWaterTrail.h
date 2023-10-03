#pragma once

#include "zzModifierSpell.h"

namespace zz
{
    class ProjectileSpell;
    class WaterTrail : public ModifierSpell
    {
    public:
        WaterTrail();
        virtual ~WaterTrail();

        virtual ProjectileSpell* ModifierProjectile(ProjectileSpell* projectile) override;
    };
}

