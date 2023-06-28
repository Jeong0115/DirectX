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
        math::Vector2 GetSize() { return mTexture->GetSize(); }
        void Clear();

        void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
        void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }

    private:
        std::shared_ptr<Shader>     mShader;
        std::shared_ptr<Texture>    mTexture;
    };

}