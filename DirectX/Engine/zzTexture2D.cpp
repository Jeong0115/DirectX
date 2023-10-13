#include "zzTexture2D.h"
#include "zzGraphicsDevice.h"

using namespace zz::graphics;
namespace zz
{
    Texture2D::Texture2D()
        : mSRV(nullptr)
        , mUAV(nullptr)
        , mSRVSlot(0)
        , mUAVslot(0)
    {
    }

    Texture2D::~Texture2D()
    {
    }
    bool Texture2D::Create(UINT width, UINT height)
    {
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = width;   
        textureDesc.Height = height; 
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        textureDesc.CPUAccessFlags = 0;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

        if (!GetDevice()->CreateTexture2d(&textureDesc, &texture))
            assert(false);
        
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

        if (!GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, &mSRV))
            assert(false);

        // UAV »ý¼º
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = textureDesc.Format;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        
        if (!GetDevice()->CreateUnorderedAccessView(texture.Get(), &uavDesc, &mUAV))
            assert(false);

        return true;
    }

    void Texture2D::SetData(void* data, UINT bufferCount)
    {
    }
    void Texture2D::GetData(void* data, UINT size)
    {
    }
    void Texture2D::BindSRV(eShaderStage stage)
    {
        GetDevice()->BindShaderResource(stage, mSRVSlot, mSRV.GetAddressOf());
    }
    void Texture2D::BindUAV()
    {
        UINT i = -1; 
        GetDevice()->BindUnorderedAccess(mUAVslot, mUAV.GetAddressOf(), &i);
    }

    void Texture2D::Clear()
    {
        ID3D11ShaderResourceView* srv = nullptr;
        GetDevice()->BindShaderResource(eShaderStage::VS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::HS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::DS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::GS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::PS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::CS, mSRVSlot, &srv);

        ID3D11UnorderedAccessView* uav = nullptr;
        UINT i = -1;
        GetDevice()->BindUnorderedAccess(mUAVslot, &uav, &i);
    }
}