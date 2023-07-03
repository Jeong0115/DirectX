#pragma once

#include "zzSolid.h"

namespace zz
{
    class Sand : public Solid
    {
    public:
        Sand();
        virtual ~Sand();

        virtual void Move() override;

        bool update = true;

    };
}