#pragma once
#include "zzGraphics.h"

namespace zz
{
    class ConstantBuffer : public GpuBuffer
    {
    public:
        ConstantBuffer(const eCBType type);
        ~ConstantBuffer();

        bool CreateConstantBuffer(size_t size);
        void SetBufferData(void* data);
        void BindConstantBuffer(eShaderStage stage);

    private:
        const eCBType mType;
    };
}

