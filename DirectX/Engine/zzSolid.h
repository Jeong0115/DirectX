#pragma once

#include "zzElement.h"

namespace zz
{
    class Solid : public Element
    {
    public:
        Solid();
        virtual ~Solid();

        virtual void Update() {};
        virtual Element* Clone() { return nullptr; };
        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth) { return false; };
    };

}
