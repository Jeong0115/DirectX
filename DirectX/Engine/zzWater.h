#pragma once

#include "zzLiquid.h"

namespace zz
{
    class Water : public Liquid
    {
    public:
        Water();
        virtual ~Water();

        virtual Element* Clone() override;

        bool update = true;

    private:
    };
}
