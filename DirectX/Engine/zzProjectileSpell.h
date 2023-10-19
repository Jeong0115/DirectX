#pragma once

#include "zzSpell.h"
#include "zzMuzzleEffect.h"
#include "zzAudioClip.h"

namespace zz
{
    class ProjectileSpell : public Spell
    {
    public:
        ProjectileSpell();
        virtual ~ProjectileSpell();

        virtual void Initialize();
        virtual void InitialSetting();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

        virtual void Dead();
        virtual ProjectileSpell* Clone();

        GameObject* GetMuzzleEffect() { return mMuzzleEffect; }

        void SetDirection(Vector3 dir) { mDirection = dir; }
        Vector3 GetDirection() { return mDirection; }
        void SetSpeedFactor(float factor) { mSpeed *= factor; }
        void SetSpeed(float speed) { mSpeed = speed; }
        void SetDamage(float damage) { mDamage = damage; }
        float GetSpeed() { return mSpeed; }
        float GetDamage() { return mDamage; }
        std::vector<struct Element>& AddTrailElement() { return mTrailElement; }

    protected:
        std::vector<struct Element> mTrailElement;

        class RigidBody*    mRigid;
        class AudioSource*  mAudio;

        MuzzleEffect*   mMuzzleEffect;
        Vector4         mPrevPos;
        Vector3         mDirection;

        float           mTime;
        float           mSpeed;
        float           mSpread;
        float           mDamage;
    };
}   
