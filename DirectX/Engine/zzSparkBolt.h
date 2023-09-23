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

        void OnCollision(class Element& element);

    private:
        class ParticleSystem*   mParticle;
        class ParticleSystem* mSubParticle;
        class ExplosionEffect*  mExplosion;
        class RigidBody*        mRigid;

        ParticleShared mShareData;

        
        float   mTime;
        float mSubParticleTime;

        Vector4 mPrevPos;

        UINT    mIndex = 0;
    };
}
