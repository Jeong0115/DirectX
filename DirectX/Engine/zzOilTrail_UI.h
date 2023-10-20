#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class OilTrail_UI : public SpellUI
    {
    public:
        OilTrail_UI();
        virtual ~OilTrail_UI();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

