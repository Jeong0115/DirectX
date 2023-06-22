#include "zzMesh.h"
#include "zzRenderer.h"
#include "zzGraphicsDevice.h"

namespace zz
{
    Mesh::Mesh()
        : Resource(eResourceType::Mesh)
        , mVertexBuffer(nullptr)
        , mIndexBuffer(nullptr)
        , mVBDesc{}
        , mIBDesc{}
        , mIndexCnt(0)
    {
    }

    Mesh::~Mesh()
    {
    }

    HRESULT Mesh::Load(const std::wstring& path)
    {
        return E_NOTIMPL;
    }

    bool Mesh::CreateVertexBuffer(void* vertexData, UINT count)
    {
        mVBDesc.ByteWidth = sizeof(renderer::Vertex) * count;
        mVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        mVBDesc.Usage = D3D11_USAGE_DEFAULT;
        mVBDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = vertexData;

        if (!graphics::GetDevice()->CreateBuffer(mVertexBuffer.GetAddressOf(), &mVBDesc, &data))
            return false;

        return true;
    }

    bool Mesh::CreateIndexBuffer(void* indexData, UINT count)
    {
        mIndexCnt = count;
        mIBDesc.ByteWidth = sizeof(UINT) * count;
        mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
        mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        mIBDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = indexData;

        if (!graphics::GetDevice()->CreateBuffer(mIndexBuffer.GetAddressOf(), &mIBDesc, &data))
            return false;

        return true;
    }

    void Mesh::BindBuffer()
    {
        UINT stride = sizeof(renderer::Vertex);
        UINT offset = 0;

        graphics::GetDevice()->BindVertexBuffer(0, mVertexBuffer.GetAddressOf(), &stride, &offset);
        graphics::GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    void Mesh::Render()
    {
        graphics::GetDevice()->DrawIndexed(mIndexCnt, 0, 0);
    }
}