#pragma once

#include "zzLiquid.h"

namespace zz
{
    class Water : public Liquid
    {
    public:
        Water();
        virtual ~Water();

        virtual void Move() override;

        bool update = true;
    };
}
