#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Smoke_Orange : public GameObject
    {
    public:
        Smoke_Orange();
        virtual ~Smoke_Orange();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetPosition(Vector3 pos);

    private:
        float mTime;
        class ParticleSystem* mParticle;
        Vector3 mPrevPos;
    };
}

