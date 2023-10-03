#include "zzWaterTrail.h"
#include "zzProjectileSpell.h"
#include "zzElement.h"
namespace zz
{
    WaterTrail::WaterTrail()
    {
    }
    WaterTrail::~WaterTrail()
    {
    }

    ProjectileSpell* WaterTrail::ModifierProjectile(ProjectileSpell* projectile)
    {
        projectile->AddTrailElement().push_back(WATER);

        return projectile;
    }
}