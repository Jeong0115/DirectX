#pragma once

#include "zzScript.h"

namespace zz
{
    class CursorScript : public Script
    {
    public:
        CursorScript();
        ~CursorScript();

        virtual void Initialize() override;
        virtual void Update() override;
    };
}

