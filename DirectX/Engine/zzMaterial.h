#pragma once

#include "zzTexture.h"
#include "zzShader.h"

namespace zz
{
    class Material : public Resource
    {
    public:
        Material();
        virtual ~Material();

        virtual HRESULT Load(const std::wstring& path) override;

        void Binds();
        math::Vector2 GetSize() { if (mTexture == nullptr) return math::Vector2::One; return mTexture->GetSize(); }
        void Clear();

        void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
        void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }
        void SetRenderingMode(eRenderingMode mode) { mMode = mode; }

        std::shared_ptr<Texture> GetTexture() { return mTexture; }
        eRenderingMode GetRenderingMode() { return mMode; }

    private:
        std::shared_ptr<Shader>     mShader;
        std::shared_ptr<Texture>    mTexture;

        eRenderingMode mMode;
    };

}