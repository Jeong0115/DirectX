#pragma once

#include "zzElement.h"

namespace zz
{
    class EmptyElement : public Element
    {
    public:
        EmptyElement();
        virtual ~EmptyElement();

        virtual Element* Clone() override;
        virtual bool InteractElement(Element* target, Position targetPos
            , bool isFinal, bool isFirst, Position lastPos, int depth) override;

        virtual void Update() override;
    };
}
