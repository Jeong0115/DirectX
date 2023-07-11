#pragma once

#include "zzSolid.h"

namespace zz
{
    class Rock : public Solid
    {
    public:
        Rock();
        virtual ~Rock();

        virtual Element* Clone() override;
    };
}
