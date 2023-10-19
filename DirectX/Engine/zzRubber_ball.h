#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class Rubber_ball : public SpellUI
    {
    public:
        Rubber_ball();
        virtual ~Rubber_ball();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

