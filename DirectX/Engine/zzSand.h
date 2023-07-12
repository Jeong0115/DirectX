#pragma once

#include "zzMovableSolid.h"

namespace zz
{
    class Sand : public MovableSolid
    {
    public:
        Sand();
        virtual ~Sand();

        virtual Element* Clone() override;

        bool update = true;

    private:
    };
}