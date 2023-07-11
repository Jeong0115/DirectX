#pragma once

#include "zzSolid.h"

namespace zz
{
    class Sand : public Solid
    {
    public:
        Sand();
        virtual ~Sand();

        virtual Element* Clone() override;

        bool update = true;

    private:
    };
}