#pragma once

#include "zzElement.h"

namespace zz
{
    class Liquid : public Element
    {
    public:
        Liquid();
        virtual ~Liquid();

        virtual void Move() = 0;
    };
}
