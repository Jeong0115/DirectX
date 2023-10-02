#pragma once

#include "zzProjectileSpell.h"

namespace zz
{
    class SparkBolt : public ProjectileSpell
    {
    public:
        SparkBolt();
        ~SparkBolt();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
       
        ProjectileSpell* Clone() override;

        void OnCollision(struct Element& element);

    private:
        class ParticleSystem*   mParticle;
        class ParticleSystem*   mSubParticle;
        class ParticleSystem*   mTailParticle;

        class ExplosionEffect*  mExplosion;
        class RigidBody*        mRigid;

        float   mTime;
        float   mSubParticleTime;
        float   mSleepTime;

        bool    mbTimerOn;

        Vector4                 mPrevPos;
        ParticleShared          mShareData;
        SineParticleShared      mTailData;
        UINT                    mIndex;
    };
}
