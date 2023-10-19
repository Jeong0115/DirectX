#pragma once

#include "zzProjectileSpell.h"
#include "zzGraphics.h"

namespace zz
{
    class BouncingBurst : public ProjectileSpell
    {
    public:
        BouncingBurst();
        virtual ~BouncingBurst();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
        virtual void Dead()         override;

        virtual void InitialSetting() override;

        virtual ProjectileSpell* Clone() override;

        void OnCollision(struct Element& element);

    private:
        class ParticleSystem* mParticle;
        ParticleShared          mSharedData;
    };
}
