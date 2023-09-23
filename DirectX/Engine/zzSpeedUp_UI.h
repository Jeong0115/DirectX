#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class SpeedUp_UI : public SpellUI
    {
    public:
        SpeedUp_UI();
        virtual ~SpeedUp_UI();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

