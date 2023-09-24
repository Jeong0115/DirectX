#pragma once

#include "zzUI.h"

namespace zz
{
    class WandMana : public UI
    {
    public:
        WandMana();
        virtual ~WandMana();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

    private:
        void createIcon();
        void createBar();

        class BarBackGround* mBar;
    };
}

