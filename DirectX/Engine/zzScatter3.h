#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class Scatter3 : public SpellUI
    {
    public:
        Scatter3();
        virtual ~Scatter3();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

