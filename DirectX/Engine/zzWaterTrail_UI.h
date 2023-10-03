#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class WaterTrail_UI : public SpellUI
    {
    public:
        WaterTrail_UI();
        virtual ~WaterTrail_UI();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

