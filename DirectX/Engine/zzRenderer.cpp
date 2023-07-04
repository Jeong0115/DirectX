#include "zzRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzMaterial.h"

#include "zzPixelGrid.h"

using namespace zz;
using namespace zz::graphics;

namespace zz::renderer
{
    Vertex vertexes[4] = {};
    std::vector<zz::Camera*> cameras = {};

    graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDSType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};

    void LoadBuffer()
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

    }
    void LoadResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");

        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"rust", L"..\\Resources\\Texture\\rust.png");

            std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
            spriteMateiral2->SetShader(spriteShader);
            spriteMateiral2->SetTexture(texture);
            ResourceManager::Insert(L"m_rust", spriteMateiral2);
        }
        {
            std::shared_ptr<Texture> texture = std::make_shared<PixelTexture>();

            std::shared_ptr<Material> pixelTexture = std::make_shared<Material>();
            pixelTexture->SetShader(spriteShader);
            pixelTexture->SetTexture(texture);
            ResourceManager::Insert(L"m_PixelTexture", pixelTexture);
        }

#pragma region MountainBG
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"T_MountainBG0", L"..\\Resources\\Texture\\parallax_clounds_00_color.png");

            std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
            spriteMateiral2->SetShader(spriteShader);
            spriteMateiral2->SetTexture(texture);
            ResourceManager::Insert(L"M_MountainBG0", spriteMateiral2);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"T_MountainBG1", L"..\\Resources\\Texture\\parallax_clounds_01.png");

            std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
            spriteMateiral2->SetShader(spriteShader);
            spriteMateiral2->SetTexture(texture);
            ResourceManager::Insert(L"M_MountainBG1", spriteMateiral2);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"T_MountainBG2", L"..\\Resources\\Texture\\parallax_mountains_02_color.png");

            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"M_MountainBG2", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"T_MountainBG3", L"..\\Resources\\Texture\\parallax_clounds_02_color.png");

            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"M_MountainBG3", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"T_MountainBG4", L"..\\Resources\\Texture\\parallax_mountains_layer_02_color.png");

            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"M_MountainBG4", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"T_MountainBG5", L"..\\Resources\\Texture\\parallax_mountains_layer_01_color.png");

            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"M_MountainBG5", spriteMateiral);
        }
#pragma endregion
#pragma region Mountain
        //LoadPixelScene("data/biome_impl/mountain/right_entrance_bottom.png", "", x, y + 512, "", true)
        //LoadPixelScene("data/biome_impl/mountain/right_bottom.png", "", x + 512 - 192, y + 512, "", true)
        //LoadPixelScene("data/biomea_impl/mountain/right_entrance.png", "data/biome_impl/mountain/right_entrance_visual.png", x, y, "data/biome_impl/mountain/right_entrance_background.png", true)

        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_0_0", L"..\\Resources\\Texture\\Mountain\\hall.png");
            PixelGrid::GetInst().SetImage(0, 0, texture);

            texture = ResourceManager::Load<Texture>(L"hall_visual_0_0", L"..\\Resources\\Texture\\Mountain\\hall_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_visual_0_0", spriteMateiral);

            texture = ResourceManager::Load<Texture>(L"hall_background_0_0", L"..\\Resources\\Texture\\Mountain\\hall_background.png");
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_background_0_0", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_instructions_0_0", L"..\\Resources\\Texture\\Mountain\\hall_instructions.png");
            PixelGrid::GetInst().SetImage(0, 0, texture);

            texture = ResourceManager::Load<Texture>(L"hall_b_0_512", L"..\\Resources\\Texture\\Mountain\\hall_b.png");
            PixelGrid::GetInst().SetImage(0, 512, texture);

            texture = ResourceManager::Load<Texture>(L"hall_b_visual_0_512", L"..\\Resources\\Texture\\Mountain\\hall_b_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_b_visual_0_512", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_br_512_512", L"..\\Resources\\Texture\\Mountain\\hall_br.png");
            PixelGrid::GetInst().SetImage(512, 512, texture);

            texture = ResourceManager::Load<Texture>(L"hall_br_visual_512_512", L"..\\Resources\\Texture\\Mountain\\hall_br_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_br_visual_512_512", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_r_512_0", L"..\\Resources\\Texture\\Mountain\\hall_r.png");
            PixelGrid::GetInst().SetImage(512, 0, texture);

            texture = ResourceManager::Load<Texture>(L"hall_r_visual_512_0", L"..\\Resources\\Texture\\Mountain\\hall_r_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_r_visual_512_0", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_bottom_-512_512", L"..\\Resources\\Texture\\Mountain\\hall_bottom.png");
            //PixelGrid::GetInst().SetImage(-512, 512, texture); //나중에 해야됩니다
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_bottom_2_552_512", L"..\\Resources\\Texture\\Mountain\\hall_bottom_2.png");
            PixelGrid::GetInst().SetImage(552, 512, texture);
        }


#pragma endregion

    }
    
    void CreateInputLayout()
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
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());
    }
    void CreateSamplerState()
    {
        D3D11_SAMPLER_DESC desc = {};
        desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        GetDevice()->CreateSamplerState(&desc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
        GetDevice()->BindSamplerState(eShaderStage::PS, 0, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        GetDevice()->CreateSamplerState(&desc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
        GetDevice()->BindSamplerState(eShaderStage::PS, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
    }
    void CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC desc = {};
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        GetDevice()->CreateRasterizeState(&desc, rasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
        GetDevice()->CreateRasterizeState(&desc, rasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        GetDevice()->CreateRasterizeState(&desc, rasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        GetDevice()->CreateRasterizeState(&desc, rasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
    }
    void CreateDepthStencilState()
    {
        D3D11_DEPTH_STENCIL_DESC desc = {};

        desc.DepthEnable = true;
        desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.StencilEnable = false;
        GetDevice()->CreateDepthStencilState(&desc, depthStencilStates[(UINT)eDSType::Less].GetAddressOf());

        desc.DepthEnable = true;
        desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.StencilEnable = false;
        GetDevice()->CreateDepthStencilState(&desc, depthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

        desc.DepthEnable = true;
        desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.StencilEnable = false;
        GetDevice()->CreateDepthStencilState(&desc, depthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

        desc.DepthEnable = false;
        desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.StencilEnable = false;
        GetDevice()->CreateDepthStencilState(&desc, depthStencilStates[(UINT)eDSType::None].GetAddressOf());
    }
    void CreateBlendState()
    {
        D3D11_BLEND_DESC desc = {};

        blendStates[(UINT)eBSType::Default] = nullptr;

        desc.AlphaToCoverageEnable = false;
        desc.IndependentBlendEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        GetDevice()->CreateBlendState(&desc, blendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

        desc.AlphaToCoverageEnable = false;
        desc.IndependentBlendEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        GetDevice()->CreateBlendState(&desc, blendStates[(UINT)eBSType::OneOne].GetAddressOf());
    }

    void SetupState()
    {
        CreateInputLayout();
        CreateSamplerState();
        CreateRasterizerState();
        CreateDepthStencilState();
        CreateBlendState();
    }

    void Initialize()
    {
        LoadBuffer();
        LoadShader();
        LoadResource();
        SetupState();
    }

    void Render()
    {
        for (Camera* camera : cameras)
        {
            if (camera == nullptr) continue;
            camera->Render();
        }
        cameras.clear();
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



