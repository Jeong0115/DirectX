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
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

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

        std::shared_ptr<AudioClip> GetAudioClip() { return mSound; }

    protected:
        std::vector<struct Element> mTrailElement;
        std::shared_ptr<AudioClip> mSound;

        class RigidBody* mRigid;

        MuzzleEffect*   mMuzzleEffect;
        Vector4         mPrevPos;
        Vector3         mDirection;

        float           mTime;
        float           mSpeed;
        float           mSpread;
        float           mDamage;
    };
}   
