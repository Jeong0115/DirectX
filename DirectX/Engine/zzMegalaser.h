#pragma once

#include "zzProjectileSpell.h"
#include "zzGraphics.h"

namespace zz
{
    class Megalaser : public ProjectileSpell
    {
    public:
        Megalaser();
        virtual ~Megalaser();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        ProjectileSpell* Clone() override;
        virtual void InitialSetting() override;

        void OnCollision(struct Element& element);

    private:
        class ParticleSystem*       mParticle;
        struct ParticleImageShared  mSharedData;

        Vector3     mOffset;
        float       mParticleTime;

        bool        mbCreate;
    };
}

