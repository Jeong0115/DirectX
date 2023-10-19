#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class Drill : public SpellUI
    {
    public:
        Drill();
        virtual ~Drill();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

