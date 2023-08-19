#include "zzStructedBuffer.h"
#include "zzGraphicsDevice.h"

namespace zz
{
    using namespace zz::graphics;

    StructedBuffer::StructedBuffer()
        : GpuBuffer()
        , mStagingBuffer(nullptr)
        , mType(eViewType::SRV)
        , mSRV(nullptr)
        , mUAV(nullptr)
        , mSize(0)
        , mStride(0)
        , mSRVSlot(0)
        , mUAVslot(0)
    {
    }

    StructedBuffer::~StructedBuffer()
    {
    }

    bool StructedBuffer::Create(UINT size, UINT stride, eViewType type, void* data, bool isStaging)
    {
        mType = type;
        mSize = size;
        mStride = stride;

        desc.ByteWidth = mSize * mStride;
        desc.StructureByteStride = mSize;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        if (mType == eViewType::UAV)
        {          
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        }
        else
        {         
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;     
        }

        if (data)
        {
            D3D11_SUBRESOURCE_DATA sub = {};
            sub.pSysMem = data;

            if (!(GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, &sub)))
                return false;
        }
        else
        {
            if (!(GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, nullptr)))
                return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.BufferEx.NumElements = mStride;
        srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;

        if (!(GetDevice()->CreateShaderResourceView(buffer.Get(), &srvDesc, mSRV.GetAddressOf())))
            return false;

        if (mType == eViewType::UAV)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Buffer.NumElements = mStride;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

            if (!GetDevice()->CreateUnordedAccessView(buffer.Get(), &uavDesc, mUAV.GetAddressOf()))
                return false;
        }

        if (isStaging)
        {
            if (CreateStagingBuffer())
                return false;
        }

        return true;
    }

    bool StructedBuffer::CreateStagingBuffer()
    {
        D3D11_BUFFER_DESC stagingDesc(desc);

        stagingDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        stagingDesc.BindFlags = 0;
        stagingDesc.Usage = D3D11_USAGE_STAGING;
        stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

        if (!GetDevice()->CreateBuffer(mStagingBuffer.GetAddressOf(), &stagingDesc, nullptr))
        {
            return false;
        }

        return true;
    }

    void StructedBuffer::SetData(void* data, UINT bufferCount)
    {
        if (mStride < bufferCount)
            Create(mSize, bufferCount, mType, data);
        else
            GetDevice()->BindStagingBuffer(mStagingBuffer.Get(), data, mSize * bufferCount);

        GetDevice()->CopyResource(buffer.Get(), mStagingBuffer.Get());
    }

    void StructedBuffer::GetData(void* data, UINT size)
    {
        GetDevice()->CopyResource(mStagingBuffer.Get(), buffer.Get());
        
        if (size == 0)
        {
            GetDevice()->ReadBuffer(mStagingBuffer.Get(), data, mSize * mStride);
        }
        else
        {
            GetDevice()->ReadBuffer(mStagingBuffer.Get(), data, size);
        }
    }

    void StructedBuffer::BindSRV(eShaderStage stage)
    {
        GetDevice()->BindShaderResource(stage, mSRVSlot, mSRV.GetAddressOf());
    }

    void StructedBuffer::BindUAV()
    {
        UINT i = -1; // 나중에 한번 확인 
        GetDevice()->BindUnorderedAccess(mUAVslot, mUAV.GetAddressOf(), &i);
    }

    void StructedBuffer::Clear()
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