#pragma once

#include "zzProjectileSpell.h"
#include "zzMegalaser.h"
namespace zz
{
    class MegalaserProj : public ProjectileSpell
    {
    public:
        MegalaserProj(Megalaser* parent, int index);
        virtual ~MegalaserProj();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void Dead();
        virtual ProjectileSpell* Clone() override;
        virtual void InitialSetting() override;

        void OnCollision(struct Element& element);

    private:
        class ExplosionEffect*  mExplosion;
        class ParticleSystem*   mParticle;

        ParticleShared          mSharedData;
        Megalaser*  mParent;

        float       mSleepTime;
        bool        mbTimerOn;

        int         mIndex;
    };
}

