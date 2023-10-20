#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class BloodTrail_UI : public SpellUI
    {
    public:
        BloodTrail_UI();
        virtual ~BloodTrail_UI();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

