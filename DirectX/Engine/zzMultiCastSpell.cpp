#include "zzMultiCastSpell.h"

namespace zz
{
    MultiCastSpell::MultiCastSpell()
        : mSpread(0.0f)
        , mCastCount(1)
    {
        mUtilityType = eUtilityType::MultiCast;
    }
    MultiCastSpell::~MultiCastSpell()
    {
    }
}