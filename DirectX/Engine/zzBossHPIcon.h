#pragma once

#include "zzUI.h"

namespace zz
{
    class BossHPIcon : public UI
    {
    public:
        BossHPIcon();
        virtual ~BossHPIcon();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    };
}

