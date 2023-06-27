#pragma once

#include "zzResource.h"
#include "zzGraphics.h"
#include "../External/DirectXTex/include/DirectXTex.h"
#include "../External/DirectXTex/include/DirectXTex.inl"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\lib\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\DirectXTex\\lib\\Release\\DirectXTex.lib")
#endif

namespace zz
{
    class Texture : public Resource
    {
    public:
        Texture();
        virtual ~Texture();

        virtual HRESULT Load(const std::wstring& path) override;
        void BindShader(eShaderStage stage, UINT startSlot);
        void Clear();

    private:
        ScratchImage mImage;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
        D3D11_TEXTURE2D_DESC mDesc;
    };
}
