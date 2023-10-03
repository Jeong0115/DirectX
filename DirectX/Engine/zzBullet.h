#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class Bullet : public SpellUI
    {
    public:
        Bullet();
        virtual ~Bullet();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

