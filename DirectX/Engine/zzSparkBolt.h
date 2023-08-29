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
        class ExplosionEffect*  mExplosion;
        class RigidBody*        mRigid;

        float   mSpeed;
        float   mTime;

        Vector4 mPrevPos;

        UINT    mIndex = 0;
    };
}
