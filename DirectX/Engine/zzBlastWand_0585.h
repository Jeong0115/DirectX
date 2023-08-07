#pragma once

#include "zzWand.h"

namespace zz
{
    class BlastWand_0585 : public Wand
    {
    public:
        BlastWand_0585();
        virtual ~BlastWand_0585();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

