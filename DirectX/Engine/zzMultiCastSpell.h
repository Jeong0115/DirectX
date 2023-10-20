#pragma once

#include "zzUtilitySpell.h"

namespace zz
{
    class MultiCastSpell : public UtilitySpell
    {
    public:
        MultiCastSpell();
        virtual ~MultiCastSpell();

        float GetSpread() { return mSpread; }
        int GetCastCount() { return mCastCount; }

    protected:
        float   mSpread;
        int     mCastCount;
    };

}



