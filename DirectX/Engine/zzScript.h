#pragma once
#include "zzComponent.h"

namespace zz
{
    class Script : public Component
    {
    public:
        Script();
        virtual ~Script();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

    private:

    };
}
