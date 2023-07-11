#pragma once

#include "zzLiquid.h"

namespace zz
{
    class Oil : public Liquid
    {
    public:
        Oil();
        virtual ~Oil();

        virtual Element* Clone() override;

        bool update = true;

    private:
    };
}
