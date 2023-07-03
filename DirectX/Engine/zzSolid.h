#pragma once

#include "zzElement.h"

namespace zz
{
    class Solid : public Element
    {
    public:
        Solid();
        virtual ~Solid();

        virtual void Move() = 0;
    };
}