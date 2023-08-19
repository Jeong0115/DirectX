#pragma once

#include "zzGraphics.h"

namespace zz
{
    class StructedBuffer : public GpuBuffer
    {
    public:
        StructedBuffer();
        ~StructedBuffer();

        bool Create(UINT size, UINT stride, eViewType type, void* data, bool isStaging = false);
        bool CreateStagingBuffer();
        void SetData(void* data, UINT bufferCount);
        void GetData(void* data, UINT size);
        void BindSRV(eShaderStage stage);
        void BindUAV();

        void SetSRVSlot(UINT slot) { mSRVSlot = slot; }
        void SetUAVSlot(UINT slot) { mUAVslot = slot; }

        void Clear();

        UINT GetSize() { return mSize; }
        UINT GetStride() { return mStride; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> mStagingBuffer;

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
        Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
        eViewType mType;

        UINT mSize;
        UINT mStride;

        UINT mSRVSlot;
        UINT mUAVslot;
    };
}
