#pragma once

#include "zzScript.h"

namespace zz
{
    class BGScript : public Script
    {
    public:
        BGScript();
        virtual ~BGScript();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

        void SetCamera(class GameObject* camera) { mainCamera = camera; }
        void SetParallaxScale(float scale) { mParallaxScale = scale; }
        void SetMoveSpeed(float moveSpeed) { mMoveSpeed = moveSpeed; }

    private:
        float mParallaxScale;
        float mMoveSpeed;
        float mWidth;
        Vector3 mLastCameraPos;
        class GameObject* mainCamera;
    };
}
