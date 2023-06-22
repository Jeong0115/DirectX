#pragma once
#include "zzResource.h"
#include "zzGraphicsDevice.h"

namespace zz
{
    class Mesh : public Resource
    {
    public:
        Mesh();
        virtual ~Mesh();

        virtual HRESULT Load(const std::wstring& path) override;

        bool CreateVertexBuffer(void* vertexData, UINT count);
        bool CreateIndexBuffer(void* indexData, UINT count);

        void BindBuffer();
        void Render();

        UINT GetIndexCount() { return mIndexCnt; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

        D3D11_BUFFER_DESC mVBDesc;
        D3D11_BUFFER_DESC mIBDesc;

        UINT mIndexCnt;
    };
}

