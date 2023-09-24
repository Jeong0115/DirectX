#pragma once

#include "zzUI.h"

namespace zz
{
    class HUD_Icon : public UI
    {
    public:
        HUD_Icon();
        virtual ~HUD_Icon();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;
    };
}

