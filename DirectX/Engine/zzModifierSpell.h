#pragma once

#include "zzUtilitySpell.h"

namespace zz
{
    class ProjectileSpell;
    class ModifierSpell : public UtilitySpell
    {
    public:
        ModifierSpell();
        virtual ~ModifierSpell();
    
        virtual ProjectileSpell* ModifierProjectile(ProjectileSpell* projectile);
    };
}

