#pragma once

#include "zzElement.h"

namespace zz
{
    class Solid : public Element
    {
    public:
        Solid();
        virtual ~Solid();

        virtual Element* Clone() = 0;
        virtual void Move() = 0;
    };
}