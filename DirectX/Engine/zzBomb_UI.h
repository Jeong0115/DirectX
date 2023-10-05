#pragma once

#include "zzSpellUI.h"

namespace zz
{
    class Bomb_UI : public SpellUI
    {
    public:
        Bomb_UI();
        virtual ~Bomb_UI();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

