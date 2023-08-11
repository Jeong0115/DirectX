#pragma once

#include "zzCamera.h"

namespace zz
{
    class UICamera : public Camera
    {
    public:
        UICamera();
        virtual ~UICamera();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void RenderUI();
        void TurnLayerMask(eUIType type, bool enable);

        void EnableUIMasks() { mUIMask.set(); }
        void DisableUIMasks() { mUIMask.reset(); }

    private:
        std::bitset<(UINT)eUIType::End>  mUIMask;
    };
}
