#pragma once

#include "zzAttackSpell.h"

namespace zz
{
    class SparkBolt : public AttackSpell
    {
    public:
        SparkBolt();
        ~SparkBolt();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
       
        void OnCollision(class Element& element);

    private:
        class ParticleSystem*   mParticle;
        class ParticleSystem* mSubParticle;
        class ExplosionEffect*  mExplosion;
        class RigidBody*        mRigid;

        ParticleShared mShareData;

        float   mSpeed;
        float   mTime;
        float mSubParticleTime;

        Vector4 mPrevPos;

        UINT    mIndex = 0;
    };
}
