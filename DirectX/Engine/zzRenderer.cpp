#include "zzRenderer.h"

using namespace zz;
using namespace zz::graphics;

namespace zz::renderer
{
    Vertex vertexes[3] = {};

    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
    graphics::ConstantBuffer* constantBuffer = nullptr;

    void SetupState()
    {
        D3D11_INPUT_ELEMENT_DESC arrLayout[2] = {};

        arrLayout[0].AlignedByteOffset = 0;
        arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        arrLayout[0].InputSlot = 0;
        arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        arrLayout[0].SemanticName = "POSITION";
        arrLayout[0].SemanticIndex = 0;

        arrLayout[1].AlignedByteOffset = 12;
        arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        arrLayout[1].InputSlot = 0;
        arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        arrLayout[1].SemanticName = "COLOR";
        arrLayout[1].SemanticIndex = 0;

        GetDevice()->CreateInputLayout(arrLayout, 2, shader->GetVSCode(), shader->GetInputLayoutAddressOf());
    }

    void LoadBuffer()
    {
        mesh = new Mesh();
        mesh->CreateVertexBuffer(vertexes, 3);

        std::vector<UINT> indexes = {};
        indexes.push_back(0);
        indexes.push_back(1);
        indexes.push_back(2);

        mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

        constantBuffer = new zz::graphics::ConstantBuffer(eCBType::Transform);
        constantBuffer->CreateConstantBuffer(sizeof(Vector4));

        Vector4 pos(0.5f, 0.0f, 0.0f, 1.0f);
        constantBuffer->SetBufferData(&pos);
        constantBuffer->BindConstantBuffer(eShaderStage::VS);
    }

    void LoadShader()
    {
        shader = new Shader();
        shader->CreateShader(eShaderStage::VS, L"TriangleVS.hlsl", "main");
        shader->CreateShader(eShaderStage::PS, L"TrianglePS.hlsl", "main");
    }

    void Initialize()
    {
        vertexes[0].pos = Vector3(0.0f, 0.5f, 0.0f);
        vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

        vertexes[1].pos = Vector3(0.5f, -0.5f, 0.0f);
        vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

        vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
        vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);  
        
       /* for (int i = 0; i <= 360; i++)
        {
            float radian = i * 3.141592f / 180.f;
            float x = std::cos(radian);
            float y = std::sin(radian);

            vertexes[index].pos = Vector3(x, y, 0.0f);
            vertexes[index++].color = Vector4(1.f, 0.f, 0.f, 1.0f);

            vertexes[index].pos = Vector3(0.0f, 0.0f, 0.0f);
            vertexes[index++].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        }*/

        LoadShader();
        LoadBuffer();
        SetupState();
    }
    void Release()
    {
        delete mesh;
        delete shader;
        delete constantBuffer;
    }
}



