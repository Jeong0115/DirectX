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
            CleanMaterial,
            OrbBlue,
        };
        Centipede();
        virtual ~Centipede();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

    private:
        void makeBody();
        void wait();
        void choice();
        void circleShoot();
        void firePillar();
        void orbBlue();
        void cleanMaterial();

        class CentipedeParticle* mCircleParticle;
        class DetectPlayer* mDetectPlayer;
        class Animator* mAnimator;
        class LimbA* limbAs[10];

        ExplosionEffect* mExplosion32Pink;

        eCentipedeState mState;

        float   mChoiceNextAction;
        float   mTime;
        bool    mbEnterAction;

        int     mActionIndex;
        int     mActionCnt;
        
    };
}

