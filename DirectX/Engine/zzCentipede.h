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
            ChoiceNextAttack,
            CircleShoot,
            FirePillar,
        };
        Centipede();
        virtual ~Centipede();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

    private:
        void makeBody();
        void battle();
        void circleShoot();
        void firePillar();

        class CentipedeParticle* mCircleParticle;
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

