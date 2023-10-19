#pragma once

#include "zzProjectileSpell.h"
#include "zzGraphics.h"

namespace zz
{
    class SparkBolt : public ProjectileSpell
    {
    public:
        SparkBolt();
        virtual ~SparkBolt();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
       
        virtual void Dead()         override;
        virtual ProjectileSpell* Clone() override;
        virtual void InitialSetting() override;

        void OnCollision(struct Element& element);

    private:
        class ParticleSystem*   mParticle;
        class ParticleSystem*   mSubParticle;
        class ExplosionEffect*  mExplosion;
        

        float   mSubParticleTime;
        float   mSleepTime;
        float   mTrailDuration;

        bool    mbTimerOn;

        ParticleShared          mShareData;
        ParticleShared          mParticleData;
        UINT                    mIndex;
    };
}
