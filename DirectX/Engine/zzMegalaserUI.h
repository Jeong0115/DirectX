#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class MegalaserUI : public SpellUI
    {
    public:
        MegalaserUI();
        virtual ~MegalaserUI();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

