#include "zzSpeedUp.h"
#include "zzProjectileSpell.h"

namespace zz
{
    SpeedUp::SpeedUp()
    {
    }
    SpeedUp::~SpeedUp()
    {
    }

    ProjectileSpell* SpeedUp::ModifierProjectile(ProjectileSpell* projectile)
    {
        projectile->SetSpeedFactor(2);

        return projectile;
    }
}