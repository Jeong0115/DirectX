#pragma once

#include "zzGraphics.h"

namespace zz
{
    class Texture2D
    {
    public:
        Texture2D();
        ~Texture2D();

        bool Create(UINT width, UINT height);

        void SetData(void* data, UINT bufferCount);
        void GetData(void* data, UINT size);

        void BindSRV(eShaderStage stage);
        void BindUAV();

        void SetSRVSlot(UINT slot) { mSRVSlot = slot; }
        void SetUAVSlot(UINT slot) { mUAVslot = slot; }

        void Clear();


    private:

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    mSRV;
        Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   mUAV;

        UINT mSRVSlot;
        UINT mUAVslot;
    };
}

