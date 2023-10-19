#pragma once

#include "zzProjectileSpell.h"

namespace zz
{
    class Luminous_drill : public ProjectileSpell
    {
    public:
        Luminous_drill();
        virtual ~Luminous_drill();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
        virtual void Dead()         override;

        virtual ProjectileSpell* Clone() override;

        virtual void InitialSetting() override;

    private:
        void deletePixel();

        bool mbFirstFrame;
    };
}

