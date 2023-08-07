#pragma once

#include "zzScript.h"

namespace zz
{
    class WandScript : public Script
    {
    public:
        WandScript();
        ~WandScript();

        virtual void Initialize() override;
        virtual void Update() override;

    private:
        class Animator* mAnimator;
    };
}
