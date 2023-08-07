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

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        bool CreateViewMatrix();
        bool CreateProjectionMatrix(eProjectionType type);
        void RegisterCameraInRenderer();

        void EnableDepthStencilState();
        void DisableDepthStencilState();

        static Matrix& GetGpuViewMatrix() { return View; }
        static void SetGpuViewMatrix(Matrix view) { View = view; }
        static Matrix& GetGpuProjectionMatrix() { return Projection; }
        static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

        float GetSize() { return mSize; }
        Matrix& GetViewMatrix() { return mView; }
        Matrix& GetProjectionMatrix() { return mProjection; }

    protected:
        static Matrix                       View;
        static Matrix                       Projection;

        Matrix                              mView;
        Matrix                              mProjection;

        eProjectionType                     mType;

        float                               mAspectRatio;
        float                               mNear;
        float                               mFar;
        float                               mSize;
    };
}
