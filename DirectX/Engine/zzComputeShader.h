#pragma once

#include "zzResource.h"
#include "zzGraphics.h"

namespace zz
{
    class ComputeShader : public Resource
    {
    public:
        ComputeShader(int x = 32, int y = 32, int z = 1);
        virtual ~ComputeShader();

        bool Create(const std::wstring& name, const std::string& methodName);
        virtual HRESULT Load(const std::wstring& path) { return S_FALSE; };
        void OnExcute();

        virtual void Binds();
        virtual void Clear();

    protected:
        Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
        Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;

        UINT mThreadGroupCountX;
        UINT mThreadGroupCountY;
        UINT mThreadGroupCountZ;

        UINT mGroupX;
        UINT mGroupY;
        UINT mGroupZ;
    };
}