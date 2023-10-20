#pragma once

#include "zzUI.h"

namespace zz
{
    class BossHPBar : public UI
    {
    public:
        BossHPBar();
        virtual ~BossHPBar();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

        void SetDead();
    private:
        UI* mHP;
        UI* mHPSlider;
        UI* mHPIcon;
    };
}

