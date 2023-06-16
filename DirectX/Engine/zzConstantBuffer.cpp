#include "zzConstantBuffer.h"
#include "zzGraphicsDevice.h"

namespace zz::graphics
{
    ConstantBuffer::ConstantBuffer(const eCBType type)
        : GpuBuffer()
        , mType(type)
    {
    }

    ConstantBuffer::~ConstantBuffer()
    {
    }

    bool ConstantBuffer::CreateConstantBuffer(size_t size)
    {
        desc.ByteWidth = (UINT)size;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, nullptr);

        return true;
    }

    void ConstantBuffer::SetBufferData(void* data)
    {
        GetDevice()->SetConstantBuffer(buffer.Get(), data, desc.ByteWidth);
    }

    void ConstantBuffer::BindConstantBuffer(eShaderStage stage)
    {
        GetDevice()->BindConstantBuffer(stage, mType, buffer.Get());
    }
}