#pragma once

#include "zzGameObject.h"

namespace zz
{
    class EndingEffect : public GameObject
    {
    public:
        EndingEffect();
        virtual ~EndingEffect();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

    private:
        class ParticleSystem*       mParticle;
        struct ParticleImageShared  mSharedData;

        class EndingView* mView;
        class Light* mLight;

        float mParticleTime;
        float mMin;
        float mMax;

        bool    mbChange;
    };
}

