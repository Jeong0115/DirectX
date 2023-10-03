#include "zzProjectileSpell.h"

namespace zz
{
    ProjectileSpell::ProjectileSpell()
        : mDirection(Vector3::Zero)
        , mSpeed(0.0f)
        , mSpread(0.0f)
        , mDamage(0.0f)
        , mMuzzleEffect(nullptr)
    {
        mType = eSpellType::Projectile;
    }
    ProjectileSpell::~ProjectileSpell()
    {
    }
    void ProjectileSpell::Initialize()
    {
    }
    void ProjectileSpell::Update()
    {
    }
    void ProjectileSpell::LateUpdate()
    {
    }
    void ProjectileSpell::Render()
    {
    }

    ProjectileSpell* ProjectileSpell::Clone()
    {
        return nullptr;
    }
}