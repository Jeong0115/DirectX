#pragma once

#include "zzComponent.h"

namespace zz
{
    class Levitation : public Component
    {
    public:
        Levitation();
        virtual ~Levitation();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetMaxEnergy(float energy) { mMaxEnergy = mCurEnergy = energy; }
        float GetLevitationEnergeRate() { return mCurEnergy / mMaxEnergy; }

    private:
        float   mMaxEnergy;
        float   mCurEnergy;
        float   mParticleTime;

        bool    mbFullEnergy;

        ParticleShared mShareData;

        class RigidBody*        mRigid;
        class ParticleSystem*   mParticle;
    };
}

