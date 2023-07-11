#include "zzSolid.h"

namespace zz
{
    Solid::Solid()
    {
        mType = eElementType::Solid;
    }
        
    Solid::~Solid()
    {

    }

    void Solid::Update()
    {
    }

    bool Solid::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position& lastPos, int depth)
    {
        return false;
    }


}