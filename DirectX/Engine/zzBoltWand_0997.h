#pragma once

#include "zzWand.h"

namespace zz
{
    class BoltWand_0997 : public Wand
    {
    public:
        BoltWand_0997();
        virtual ~BoltWand_0997();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}
