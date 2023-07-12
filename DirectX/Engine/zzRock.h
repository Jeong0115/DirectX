#pragma once

#include "zzImmovableSolid.h"

namespace zz
{
    class Rock : public ImmovableSolid
    {
    public:
        Rock();
        virtual ~Rock();

        virtual Element* Clone() override;
    };
}
