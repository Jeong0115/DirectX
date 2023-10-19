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

		void SetGroupCount(UINT x, UINT y, UINT z) { mGroupX = x; mGroupY = y; mGroupZ = z; }
        std::tuple<UINT, UINT, UINT> GetGroupCount() { return { mGroupX, mGroupY, mGroupZ }; }

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
