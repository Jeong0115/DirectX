#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class LightBullet : public SpellUI
    {
    public:
        LightBullet();
        virtual ~LightBullet();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

