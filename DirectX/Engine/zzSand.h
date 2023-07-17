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
        virtual bool ReceiveHeat(int heat) override;

        bool update = true;

    private:
    };
}