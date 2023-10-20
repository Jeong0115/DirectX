#include "zzBloodTrail.h"
#include "zzProjectileSpell.h"
#include "zzElement.h"

namespace zz
{
    BloodTrail::BloodTrail()
    {
    }
    BloodTrail::~BloodTrail()
    {
    }
    ProjectileSpell* BloodTrail::ModifierProjectile(ProjectileSpell* projectile)
    {
        projectile->AddTrailElement().push_back(BLOOD);

        return projectile;
    }
}