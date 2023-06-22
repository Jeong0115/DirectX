#include "zzRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzMaterial.h"

using namespace zz;
using namespace zz::graphics;

namespace zz::renderer
{
    Vertex vertexes[4] = {};

    graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[(UINT)eSamplerType::End] = {};

    void SetupState()
    {
        D3D11_INPUT_ELEMENT_DESC arrLayout[3] = {};

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

        arrLayout[2].AlignedByteOffset = 28;
        arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
        arrLayout[2].InputSlot = 0;
        arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        arrLayout[2].SemanticName = "TEXCOORD";
        arrLayout[2].SemanticIndex = 0;

        std::shared_ptr<Shader> shader = ResourceManager::Find<Shader>(L"TriangleShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"SpriteShader");
        GetDevice()->CreateInputLayout(arrLayout, 3 , shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        //std::shared_ptr<ID3D11SamplerState> samplerState = std::make_shared<ID3D11SamplerState>();

        D3D11_SAMPLER_DESC desc = {};
        desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        GetDevice()->CreateSamplerState(&desc, samplerState[(UINT)eSamplerType::Point].GetAddressOf());
        GetDevice()->BindSamplerState(eShaderStage::PS, 0, samplerState[(UINT)eSamplerType::Point].GetAddressOf());

        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        GetDevice()->CreateSamplerState(&desc, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
        GetDevice()->BindSamplerState(eShaderStage::PS, 1, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());

        //GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());
    }

    void LoadBuffer()
    {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        mesh->CreateVertexBuffer(vertexes, 4);
        ResourceManager::Insert(L"RectMesh", mesh);

        std::vector<UINT> indexes = {};

        indexes.push_back(0);
        indexes.push_back(1);
        indexes.push_back(2);

        indexes.push_back(0);
        indexes.push_back(2);
        indexes.push_back(3);

        mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

        constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
        constantBuffer[(UINT)eCBType::Transform]->CreateConstantBuffer(sizeof(TransformCB));

        //Vector4 pos(0.5f, 0.0f, 0.0f, 1.0f);
        //constantBuffer->SetBufferData(&pos);
        //constantBuffer->BindConstantBuffer(eShaderStage::VS);
    }

    void LoadShader()
    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->CreateShader(eShaderStage::VS, L"TriangleVS.hlsl", "main");
        shader->CreateShader(eShaderStage::PS, L"TrianglePS.hlsl", "main");
        ResourceManager::Insert(L"TriangleShader", shader);

        std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
        spriteShader->CreateShader(eShaderStage::VS, L"SpriteVS.hlsl", "main");
        spriteShader->CreateShader(eShaderStage::PS, L"SpritePS.hlsl", "main");
        ResourceManager::Insert(L"SpriteShader", spriteShader);

        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"Charging1", L"..\\Resources\\Texture\\Charging.png");

        std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
        spriteMateiral->SetShader(spriteShader);
        spriteMateiral->SetTexture(texture);
        ResourceManager::Insert(L"SpriteMaterial", spriteMateiral);
    }

    void Initialize()
    {
        vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
        vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        vertexes[0].uv = Vector2(0.0f, 0.0f);

        vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
        vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        vertexes[1].uv = Vector2(1.0f, 0.0f);

        vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
        vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
        vertexes[2].uv = Vector2(1.0f, 1.0f);

        vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
        vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        vertexes[3].uv = Vector2(0.0f, 1.0f);
        
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

        LoadBuffer();
        LoadShader();
        SetupState();


        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"Charging1");
        texture->BindShader(eShaderStage::PS, 0);

    }
    void Release()
    {
        for (ConstantBuffer* buff : constantBuffer)
        {
            if (buff == nullptr)
                continue;

            delete buff;
            buff = nullptr;
        }
    }
}



