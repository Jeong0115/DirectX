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

        void SetLightColor(Vector4 color) { mLightColor = color; }
        void SetLightColor(float r, float g, float b, float a) { mLightColor = Vector4(r, g, b, a); }

        void SetAfterimageEffect(float duration) { mAfterimageDurtaion = duration; mbAfterimageEffect = true; }
        void SetLightType(int type);

        void TrunOff() { mbLight = false; }
        void TrunOn() { mbLight = true; }

    private:
        std::shared_ptr<class Material> mMaterial;
        class Transform*    mTransform;


        Vector3 mLightScale;
        Vector4 mLightColor;

        float   mAfterimageDurtaion;

        bool    mbAfterimageEffect;
        bool    mbLight;

        std::vector<std::pair<Matrix, float>> mAfterimageEffect;
    };
}

