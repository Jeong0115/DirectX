#pragma once

#include "zzElement.h"

namespace zz
{
    class Solid : public Element
    {
    public:
        Solid();
        virtual ~Solid();

        virtual void Update() = 0;
        virtual Element* Clone() = 0;
        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth) = 0;
    };

}
