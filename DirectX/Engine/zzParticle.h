#pragma once

#include "zzGameObject.h"

namespace zz
{
    class TempParticle : public GameObject
    {
    public:
        TempParticle();
        virtual ~TempParticle();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        void SetVelocity(Vector3 vel) { mVelocity = vel; }
        void SetLifeTime(float time) { mLifeTime = time; }

    protected:
        Vector3 mOffset;
        Vector3 mOffsetMin;
        Vector3 mOffsetMax;
        Vector3 mVelocityMin;
        Vector3 mVelocityMax;
        Vector3 mVelocity;

        float   mAirForce;
        float   mLifeTime;

        float mPassedTime;
    };
}
