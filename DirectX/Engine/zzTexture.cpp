#include "zzTexture.h"
#include "zzGraphicsDevice.h"

#include "zzPixelWorld.h"
namespace zz
{
    Texture::Texture()
        : Resource(eResourceType::Texture)
        , mImage{}
        , mTexture(nullptr)
        , mSRV(nullptr)
        , mDesc{}
    {
        
    }

    Texture::~Texture()
    {
    }

    HRESULT Texture::Load(const std::wstring& path)
    {
        wchar_t szExtension[50] = {};
        _wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 50);

        std::wstring extension = szExtension;
        if (extension == L".dds" || extension == L".DDS")
        {
            if (FAILED(LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
                return S_FALSE;
        }
        else if (extension == L".tga" || extension == L".TGA")
        {
            if (FAILED(LoadFromTGAFile(path.c_str(), nullptr, mImage)))
                return S_FALSE;
        }
        else // WIC (png, jpg, jpeg, bmp )
        {
            if (FAILED(LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
                return S_FALSE;
        }


        CreateShaderResourceView
        (
            graphics::GetDevice()->GetID3D11Device()
            , mImage.GetImages()
            , mImage.GetImageCount()
            , mImage.GetMetadata()
            , mSRV.GetAddressOf()
        );
        
        mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());

        return S_OK;
    }

    void Texture::BindShader(eShaderStage stage, UINT startSlot)
    {
        graphics::GetDevice()->BindShaderResource(stage, startSlot, mSRV.GetAddressOf());      
    }

    void Texture::Clear()
    {
        ID3D11ShaderResourceView* srv = nullptr;

        graphics::GetDevice()->BindShaderResource(eShaderStage::VS, 0, &srv);
        graphics::GetDevice()->BindShaderResource(eShaderStage::DS, 0, &srv);
        graphics::GetDevice()->BindShaderResource(eShaderStage::GS, 0, &srv);
        graphics::GetDevice()->BindShaderResource(eShaderStage::HS, 0, &srv);
        graphics::GetDevice()->BindShaderResource(eShaderStage::CS, 0, &srv);
        graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 0, &srv);
    }


    PixelTexture::PixelTexture()
    {
        std::vector<uint8_t>& pixelTexture = PixelWorld::GetPixelColor();

        TexMetadata metadata;
        metadata.width = 2048;
        metadata.height = 2048;
        metadata.depth = 1;
        metadata.arraySize = 1;
        metadata.mipLevels = 1;
        metadata.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        metadata.dimension = TEX_DIMENSION_TEXTURE2D;

        HRESULT hr2 = mImage.Initialize(metadata, CP_FLAGS_NONE);

        Image image;
        image.width = 2048;
        image.height = 2048;
        image.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        image.rowPitch = image.width * sizeof(uint32_t);  // for DXGI_FORMAT_R8G8B8A8_UNORM
        image.slicePitch = image.rowPitch * image.height;
        image.pixels = pixelTexture.data();

        mImage.InitializeFromImage(image);

        HRESULT hr = CreateShaderResourceView
        (
            graphics::GetDevice()->GetID3D11Device()
            , mImage.GetImages()
            , mImage.GetImageCount()
            , mImage.GetMetadata()
            , mSRV.GetAddressOf()
        );

        mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());
    }
    PixelTexture::~PixelTexture()
    {
    }

    void PixelTexture::BindShader(eShaderStage stage, UINT startSlot)
    {
        std::vector<uint8_t>& pixelTexture = PixelWorld::GetPixelColor();
        zz::graphics::GetDevice()->UpdateSubresource(mTexture.Get(), pixelTexture.data());
        Texture::BindShader(stage, startSlot);
    }
}