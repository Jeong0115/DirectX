#pragma once

#include "zzCamera.h"

namespace zz
{
    class MainCamera : public Camera
    {
    public:
        MainCamera();
        virtual ~MainCamera();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void TurnLayerMask(eLayerType type, bool enable = true);
        void EnableLayerMasks()     { mLayerMask.set(); }
        void DisableLayerMasks()    { mLayerMask.reset(); }

        void SortGameObjects();
        void RenderOpaque();
        void RenderCutOut();
        void RenderTransparent();



    private:
        std::bitset<(UINT)eLayerType::End>  mLayerMask;

        std::vector<GameObject*>            mOpaqueGameObjects;
        std::vector<GameObject*>            mCutOutGameObjects;
        std::vector<GameObject*>            mTransparentGameObjects;
    };
}
