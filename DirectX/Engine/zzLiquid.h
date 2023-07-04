#pragma once

#include "zzElement.h"

namespace zz
{
    class Liquid : public Element
    {
    public:
        Liquid();
        virtual ~Liquid();

        virtual Element* Clone() = 0;
        virtual void Move() = 0;
    };
}
