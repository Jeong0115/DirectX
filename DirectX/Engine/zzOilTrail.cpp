#include "zzOilTrail.h"
#include "zzProjectileSpell.h"
#include "zzElement.h"

namespace zz
{
    OilTrail::OilTrail()
    {
    }
    OilTrail::~OilTrail()
    {
    }
    ProjectileSpell* OilTrail::ModifierProjectile(ProjectileSpell* projectile)
    {
        projectile->AddTrailElement().push_back(OIL);

        return projectile;
    }
}