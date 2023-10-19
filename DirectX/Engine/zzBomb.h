#pragma once

#include "zzProjectileSpell.h"

namespace zz
{
    class Bomb : public ProjectileSpell
    {
    public:
        Bomb();
        virtual ~Bomb();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void InitialSetting() override;
        Bomb* Clone() override;


    private:
    };
}