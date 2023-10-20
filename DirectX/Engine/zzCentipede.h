#pragma once

#include "zzGameObject.h"
#include "zzExplosionEffect.h"

namespace zz
{
    class Centipede : public GameObject
    {
    public:
        enum class eCentipedeState
        {
            Sleep,
            Wait,
            ChoiceNextAttack,
            CircleShoot,
            FirePillar,
            BugBlue,
            CleanMaterial,
            OrbBlue,
        };
        Centipede();
        virtual ~Centipede();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void Move();

        void DeadEvent();
        void HitEvent();

        void Awake();
        void NextAnimation();

    private:
        void makeBody();
        void wait();
        void choice();
        void circleShoot();
        void firePillar();
        void bugBlue();
        void orbBlue();
        void cleanMaterial();

        class CentipedeParticle*    mCircleParticle;
        class HealthPoint*          mHealthPoint;
        class DetectPlayer*         mDetectPlayer;
        class RigidBody*            mRigid;
        class Animator*             mAnimator;
        class LimbA*                limbAs[10];
        std::vector<class Tail1*> mTail1s;
        std::vector<class Tail2*> mTail2s;
        class BossHPBar*    mHP;
        class AudioSource*  mAttackAudio;
        class AudioSource*  mAudio;

        ExplosionEffect* mExplosion32Pink;

        eCentipedeState mState;
      
        Vector3 mInitialPos;
        Vector3 mTargetPos;
        Vector3 mVelocity;

        float   mChoiceNextAction;
        float   mHitFlashTime;
        float   mTime;
        float   mMoveTime;

        bool    mbEnterAction;
        bool    mbDead;

        int     mActionIndex;
        int     mActionCnt;
        
    };
}

