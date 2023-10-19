#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Zombie_weak : public GameObject
    {
    public:
        enum class eMonsterState
        {
            Freedom,
            FollowPlayer,
            Battle,
        };

        Zombie_weak();
        virtual ~Zombie_weak();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

        void playIdleAnimation();

        void DeadEvent();
        void HitEvent();

    private:
        void freedom();
        void followPlayer();
        void battle();

        class PixelCollider*    mPxCollider;
        class HealthPoint*      mHealPoint;
        class DetectPlayer*     mDetectPlayer;
        class Animator*         mAnimator;
        class RigidBody*        mRigid;

        eMonsterState mState;

        Vector3 mPxColliderScale;
        Vector3 mTip;

        float   mChoiceNextAction;
        float   mDirection;
        float   mHitFlashTime;
        float   mDetectPlayerTime;
        float   mReloadTime;

        int     mActionIndex;

        bool    mbEnterAction;
        bool    mbJump;
    };
}

