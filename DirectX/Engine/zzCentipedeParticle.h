#pragma once

#include "zzGameObject.h"

namespace zz
{
    class CentipedeParticle : public GameObject
    {
    public:
        CentipedeParticle();
        virtual ~CentipedeParticle();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void Play();

    private:
        class ParticleSystem* mParticle;    
        struct ParticleCircleShared mShareData;

        bool mbCreate;
        float mTime;
    };
}

