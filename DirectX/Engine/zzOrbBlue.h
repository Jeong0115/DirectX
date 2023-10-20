#pragma once
#include "zzMonsterAttack.h"

namespace zz
{
    class OrbBlue : public MonsterAttack
    {
    public:
        OrbBlue(int type);
        virtual ~OrbBlue();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

    private:
        virtual void dead() override;

        class AudioSource* mAudio;

        class RigidBody* mRigid;
        ExplosionEffect* mEffect;

        Vector3 mOrbitalCenter;

        float mOrbitalTime;
        int mType;
    };
}

