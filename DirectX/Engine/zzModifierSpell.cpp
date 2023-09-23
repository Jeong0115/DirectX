#include "zzModifierSpell.h"
#include "zzProjectileSpell.h"

namespace zz
{
    ModifierSpell::ModifierSpell()
    {
        mUtilityType = eUtilityType::Modifier;
    }

    ModifierSpell::~ModifierSpell()
    {
    }

    ProjectileSpell* ModifierSpell::ModifierProjectile(ProjectileSpell* projectile)
    {
        return nullptr;
    }
}