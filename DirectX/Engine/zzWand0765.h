#pragma once

#include "zzWand.h"

namespace zz
{
    class Wand0765 : public Wand
    {
    public:
        Wand0765();
        virtual ~Wand0765();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void UseEquipment() override;
    };
}