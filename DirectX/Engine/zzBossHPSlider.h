#pragma once

#include "zzUI.h"

namespace zz
{
    class BossHPSlider : public UI
    {
    public:
        BossHPSlider();
        virtual ~BossHPSlider();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other) override;
        virtual void OnCollisionExit(GameObject* other) override;

        void OnEvent(const struct EvenetData& data);

    private:
        float mHPRate;
    };
}

