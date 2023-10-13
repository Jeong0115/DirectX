#pragma once
#include "zzMonsterAttack.h"

namespace zz
{
    class Grenade :public MonsterAttack
    {
    public:
        Grenade();
        virtual ~Grenade();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

        void OnCollision(struct Element& element);

    private:
        virtual void dead() override;

        class ParticleSystem*   mParticle;
        class RigidBody*        mRigid;
        class Light*            mLight;

        struct ParticleShared   mShared;

        ExplosionEffect* mEffect;

        Vector3 mPrevPos;
        float mCnt;
        float mSpareTime;
        int mStep;
    };
}

