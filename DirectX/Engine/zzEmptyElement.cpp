#include "zzEmptyElement.h"

namespace zz
{
    EmptyElement::EmptyElement()
    {
        mColor = ElementColor::Empty;
        mType = eElementType::Empty;
    }
    EmptyElement::~EmptyElement()
    {

    }
    Element* EmptyElement::Clone()
    {
        return new EmptyElement();
    }
    bool EmptyElement::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth)
    {
        return false;
    }
    void EmptyElement::Update()
    {
        return;
    }
}