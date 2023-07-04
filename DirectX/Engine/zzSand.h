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
        virtual Element* Clone() override;

        bool update = true;

    private:
        float mSpeed;
        float temp;

        float y;
    };
}