#pragma once

#include "zzComponent.h"

namespace zz
{
    class Light : public Component
    {
    public:
        Light();
        virtual ~Light();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetLightScale(Vector3 scale) { mLightScale = scale; }
        void SetLightScale(float x, float y, float z) { mLightScale = Vector3(x, y, z); }

    private:
        class Transform* mTransform;

        Vector3 mLightScale;
    };
}

