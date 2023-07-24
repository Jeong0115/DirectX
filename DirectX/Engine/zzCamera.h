#pragma once

#include "zzComponent.h"

namespace zz
{
    class Camera : public Component
    {
    public:
        enum class eProjectionType
        {
            Perpective,
            OrthoGraphic,
            None,
        };

        Camera();
        ~Camera();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        bool CreateViewMatrix();
        bool CreateProjectionMatrix(eProjectionType type);
        void RegisterCameraInRenderer();

        void TurnLayerMask(eLayerType type, bool enable = true);
        void EnableLayerMasks()     { mLayerMask.set(); }
        void DisableLayerMasks()    { mLayerMask.reset(); }

        void SortGameObjects();
        void RenderOpaque();
        void RenderCutOut();
        void RenderTransparent();

        void EnableDepthStencilState();
        void DisableDepthStencilState();

        static Matrix& GetGpuViewMatrix()                       { return View; }
        static void SetGpuViewMatrix(Matrix view)               { View = view; }
        static Matrix& GetGpuProjectionMatrix()                 { return Projection; }
        static void SetGpuProjectionMatrix(Matrix projection)   { Projection = projection; }

        float GetSize()                 { return mSize; }
        Matrix& GetViewMatrix()         { return mView; }
        Matrix& GetProjectionMatrix()   { return mProjection; }

    private:
        static Matrix                       View;
        static Matrix                       Projection;

        Matrix                              mView;
        Matrix                              mProjection;

        std::bitset<(UINT)eLayerType::End>  mLayerMask;

        std::vector<GameObject*>            mOpaqueGameObjects;
        std::vector<GameObject*>            mCutOutGameObjects;
        std::vector<GameObject*>            mTransparentGameObjects;

        eProjectionType                     mType;

        float                               mAspectRatio;
        float                               mNear;
        float                               mFar;
        float                               mSize;
    };
}
