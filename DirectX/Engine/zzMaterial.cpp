#include "zzMaterial.h"

namespace zz
{
    Material::Material()
        : Resource(eResourceType::Material)
        , mShader(nullptr)
        , mTexture(nullptr)
    {
    }

    Material::~Material()
    {
    }

    HRESULT Material::Load(const std::wstring& path)
    {
        return E_NOTIMPL;
    }

    void Material::Binds()
    {
        mTexture->BindShader(eShaderStage::PS, 0); // 나중에 함수 이름 바꾸자
        mShader->BindShaders();
    }
}