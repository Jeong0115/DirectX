#include "zzRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzMaterial.h"
#include "zzParticleShader.h"

#include "zzPixelWorld.h"

using namespace zz;
using namespace zz::graphics;

namespace zz::renderer
{
    zz::Camera* mainCamera = nullptr;
    zz::Camera* uiCamera = nullptr;
    std::vector<zz::Camera*> cameras = {};
    std::vector<DebugMesh> debugMeshs = {};

    ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDSType::End] = {};
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};

    void LoadUIResource();
    void LoadWandTextureResource();
    void LoadSpellResource();
    void LoadEffectResource();
    void LoadParticleResource();
    void LoadMonsterResource();

    void LoadBuffer()
    {
        Vertex vertexes[4] = {};
        Vertex debugVertexes[4] = {};

        {
            vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
            vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
            vertexes[0].uv = Vector2(0.0f, 0.0f);

            vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
            vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
            vertexes[1].uv = Vector2(1.0f, 0.0f);

            vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
            vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
            vertexes[2].uv = Vector2(0.0f, 1.0f);

            vertexes[3].pos = Vector3(0.5f, -0.5f, 0.0f);
            vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
            vertexes[3].uv = Vector2(1.0f, 1.0f);
        }

        debugVertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
        debugVertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        debugVertexes[0].uv = Vector2(0.0f, 0.0f);

        debugVertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
        debugVertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        debugVertexes[1].uv = Vector2(1.0f, 0.0f);

        debugVertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
        debugVertexes[2].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        debugVertexes[2].uv = Vector2(1.0f, 1.0f);

        debugVertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
        debugVertexes[3].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        debugVertexes[3].uv = Vector2(0.0f, 1.0f);


        {
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
            mesh->CreateVertexBuffer(vertexes, 4);
            ResourceManager::Insert(L"RectMesh", mesh);

            std::vector<UINT> indexes = {};

            indexes.push_back(0);
            indexes.push_back(1);
            indexes.push_back(2);

            indexes.push_back(1);
            indexes.push_back(3);
            indexes.push_back(2);

            mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
        }

        {
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
            mesh->CreateVertexBuffer(debugVertexes, 4);

            std::vector<UINT> indexes = { 0, 1, 2, 3 ,0 };
            mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

            ResourceManager::Insert(L"DebugMesh", mesh);
        }

        debugVertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        debugVertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        debugVertexes[2].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        debugVertexes[3].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        mesh->CreateVertexBuffer(debugVertexes, 4);

        std::vector<UINT> indexes = { 0, 1, 2, 3 ,0 };
        mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

        ResourceManager::Insert(L"RedDebugMesh", mesh);

        {
            std::vector<Vertex> pointVertexes = {};
            std::vector<UINT> pointIndexes = {};

            Vertex v = {};
            v.pos = Vector3(0.0f, 0.0f, 0.0f);
            pointVertexes.push_back(v);
            pointIndexes.push_back(0);
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
            mesh->CreateVertexBuffer(pointVertexes.data(), pointVertexes.size());
            mesh->CreateIndexBuffer(pointIndexes.data(), pointIndexes.size());
            mesh->SetName(L"PointMesh");
            ResourceManager::Insert(L"PointMesh", mesh);
        }


        {
            //std::vector<Vertex> lineVertexes = {};
            //std::vector<UINT> lineIndexes = {};

            //Vertex v = {};
            //v.pos = Vector3(0.0f, 0.0f, 0.0f);
            //v
            //lineVertexes.push_back(v);
            //lineIndexes.push_back(0);
            //std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
            //mesh->CreateVertexBuffer(pointVertexes.data(), pointVertexes.size());
            //mesh->CreateIndexBuffer(pointIndexes.data(), pointIndexes.size());
            //mesh->SetName(L"PointMesh");
            //ResourceManager::Insert(L"PointMesh", mesh);
        }

        constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
        constantBuffer[(UINT)eCBType::Transform]->CreateConstantBuffer(sizeof(TransformCB));

        constantBuffer[(UINT)eCBType::Color] = new ConstantBuffer(eCBType::Color);
        constantBuffer[(UINT)eCBType::Color]->CreateConstantBuffer(sizeof(ColorCB));

        constantBuffer[(UINT)eCBType::Animator] = new ConstantBuffer(eCBType::Animator);
        constantBuffer[(UINT)eCBType::Animator]->CreateConstantBuffer(sizeof(AnimatorCB));

        constantBuffer[(UINT)eCBType::Flip] = new ConstantBuffer(eCBType::Flip);
        constantBuffer[(UINT)eCBType::Flip]->CreateConstantBuffer(sizeof(FlipCB));

        constantBuffer[(UINT)eCBType::Particle] = new ConstantBuffer(eCBType::Particle);
        constantBuffer[(UINT)eCBType::Particle]->CreateConstantBuffer(sizeof(ParticleCB));

        constantBuffer[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
        constantBuffer[(UINT)eCBType::Noise]->CreateConstantBuffer(sizeof(NoiseCB));
    }
    void LoadShader()
    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->CreateShader(eShaderStage::VS, L"DebugVS.hlsl", "main");
        shader->CreateShader(eShaderStage::PS, L"DebugPS.hlsl", "main");
        shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        shader->SetRSState(eRSType::WireframeNone);
        ResourceManager::Insert(L"DebugShader", shader);

        std::shared_ptr<Shader> triangleShader = std::make_shared<Shader>();
        triangleShader->CreateShader(eShaderStage::VS, L"TriangleVS.hlsl", "main");
        triangleShader->CreateShader(eShaderStage::PS, L"TrianglePS.hlsl", "main");
        triangleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        triangleShader->SetRSState(eRSType::WireframeNone);
        ResourceManager::Insert(L"TriangleShader", triangleShader);

        std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
        spriteShader->CreateShader(eShaderStage::VS, L"SpriteVS.hlsl", "main");
        spriteShader->CreateShader(eShaderStage::PS, L"SpritePS.hlsl", "main");
        ResourceManager::Insert(L"SpriteShader", spriteShader);

        std::shared_ptr<Shader> spriteAnimationShader = std::make_shared<Shader>();
        spriteAnimationShader->CreateShader(eShaderStage::VS, L"SpriteAnimationVS.hlsl", "main");
        spriteAnimationShader->CreateShader(eShaderStage::PS, L"SpriteAnimationPS.hlsl", "main");
        ResourceManager::Insert(L"SpriteAnimationShader", spriteAnimationShader);

        std::shared_ptr<ParticleShader> psSystemShader = std::make_shared<ParticleShader>();
        psSystemShader->Create(L"ProjectileParticleCS.hlsl", "main");
        ResourceManager::Insert(L"ProjectileParticleCS", psSystemShader);

        std::shared_ptr<ParticleShader> psSystemShader1 = std::make_shared<ParticleShader>();
        psSystemShader1->Create(L"ParticleCS.hlsl", "main");
        ResourceManager::Insert(L"ParticleCS", psSystemShader1);

        std::shared_ptr<Shader> paritcleShader = std::make_shared<Shader>();
        paritcleShader->CreateShader(eShaderStage::VS, L"ParticleVS.hlsl", "main");
        paritcleShader->CreateShader(eShaderStage::GS, L"ParticleGS.hlsl", "main");
        paritcleShader->CreateShader(eShaderStage::PS, L"ParticlePS.hlsl", "main");
        paritcleShader->SetRSState(eRSType::SolidNone);
        paritcleShader->SetDSState(eDSType::NoWrite);
        paritcleShader->SetBSState(eBSType::AlphaBlend);
        paritcleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
        ResourceManager::Insert(L"ParticleShader", paritcleShader);

        std::shared_ptr<ParticleShader> psSystemAnimationShader = std::make_shared<ParticleShader>();
        psSystemAnimationShader->Create(L"ParticleAnimationCS.hlsl", "main");
        ResourceManager::Insert(L"ParticleAnimationCS", psSystemAnimationShader);

        std::shared_ptr<Shader> paritcleAnimationShader = std::make_shared<Shader>();
        paritcleAnimationShader->CreateShader(eShaderStage::VS, L"ParticleAnimationVS.hlsl", "main");
        paritcleAnimationShader->CreateShader(eShaderStage::GS, L"ParticleAnimationGS.hlsl", "main");
        paritcleAnimationShader->CreateShader(eShaderStage::PS, L"ParticleAnimationPS.hlsl", "main");
        paritcleAnimationShader->SetRSState(eRSType::SolidNone);
        paritcleAnimationShader->SetDSState(eDSType::NoWrite);
        paritcleAnimationShader->SetBSState(eBSType::AlphaBlend);
        paritcleAnimationShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
        ResourceManager::Insert(L"ParticleAnimationShader", paritcleAnimationShader);

    }
    void LoadResource()
    {
        LoadParticleResource();
        LoadUIResource();
        LoadWandTextureResource();
        LoadEffectResource();
        LoadSpellResource();
        LoadMonsterResource();

        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");

        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"mouse_cursor", L"..\\Resources\\Texture\\Mouse\\mouse_cursor.png");
            std::shared_ptr<Material> material = std::make_shared<Material>();

            material = std::make_shared<Material>();
            material->SetShader(spriteShader);
            material->SetTexture(texture);
            ResourceManager::Insert(L"m_mouse_cursor", material);
        }

        {
            std::shared_ptr<Shader> debugShader = ResourceManager::Find<Shader>(L"DebugShader");
            std::shared_ptr<Material> material = std::make_shared<Material>();

            material = std::make_shared<Material>();
            material->SetShader(debugShader);
            ResourceManager::Insert(L"DebugMaterial", material);
        }

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
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"background_coalmine", L"..\\Resources\\Texture\\Coalmine\\background_coalmine.png");

            std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
            spriteMateiral2->SetShader(spriteShader);
            spriteMateiral2->SetTexture(texture);
            ResourceManager::Insert(L"m_background_coalmine", spriteMateiral2);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"rust", L"..\\Resources\\Texture\\rust.png");

            std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
            spriteMateiral2->SetShader(spriteShader);
            spriteMateiral2->SetTexture(texture);
            ResourceManager::Insert(L"m_rust", spriteMateiral2);
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
            std::shared_ptr<Texture> texture_visual = ResourceManager::Load<Texture>(L"hall_visual_0_0", L"..\\Resources\\Texture\\Mountain\\hall_visual.png");

            PixelWorld::SetImage(0, 0, texture, texture_visual);

            
      /*      std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture_visual);
            ResourceManager::Insert(L"m_hall_visual_0_0", spriteMateiral);*/

            texture = ResourceManager::Load<Texture>(L"hall_background_0_0", L"..\\Resources\\Texture\\Mountain\\hall_background.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_background_0_0", spriteMateiral);

            texture = ResourceManager::Load<Texture>(L"left_entrance_background_-512_0", L"..\\Resources\\Texture\\Mountain\\left_entrance_background.png");
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_left_entrance_background_-512_0", spriteMateiral);
        }
        {
            //std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_instructions_0_0", L"..\\Resources\\Texture\\Mountain\\hall_instructions.png");
            //PixelGrid::GetInst().SetImage(0, 0, texture);

            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_b_0_512", L"..\\Resources\\Texture\\Mountain\\hall_b.png");
            std::shared_ptr<Texture> texture_visual = ResourceManager::Load<Texture>(L"hall_b_visual_0_512", L"..\\Resources\\Texture\\Mountain\\hall_b_visual.png");
            PixelWorld::SetImage(0, 512, texture, texture_visual);

            texture = ResourceManager::Load<Texture>(L"hall_b_visual_0_512", L"..\\Resources\\Texture\\Mountain\\hall_b_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_b_visual_0_512", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_br_512_512", L"..\\Resources\\Texture\\Mountain\\hall_br.png");
            std::shared_ptr<Texture> texture_visual = ResourceManager::Load<Texture>(L"hall_br_visual_512_512", L"..\\Resources\\Texture\\Mountain\\hall_br_visual.png");
            PixelWorld::SetImage(512, 512, texture, texture_visual);

            texture = ResourceManager::Load<Texture>(L"hall_br_visual_512_512", L"..\\Resources\\Texture\\Mountain\\hall_br_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_br_visual_512_512", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_r_512_0", L"..\\Resources\\Texture\\Mountain\\hall_r.png");
            std::shared_ptr<Texture> texture_visual = ResourceManager::Load<Texture>(L"hall_r_visual_512_0", L"..\\Resources\\Texture\\Mountain\\hall_r_visual.png");
            PixelWorld::SetImage(512, 0, texture, texture_visual);

            texture = ResourceManager::Load<Texture>(L"hall_r_visual_512_0", L"..\\Resources\\Texture\\Mountain\\hall_r_visual.png");
            std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
            spriteMateiral = std::make_shared<Material>();
            spriteMateiral->SetShader(spriteShader);
            spriteMateiral->SetTexture(texture);
            ResourceManager::Insert(L"m_hall_r_visual_512_0", spriteMateiral);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_bottom_-512_512", L"..\\Resources\\Texture\\Mountain\\hall_bottom.png");
            //PixelWorld::GetInst().SetImage(-512, 512, texture); //나중에 해야됩니다
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"hall_bottom_2_552_512", L"..\\Resources\\Texture\\Mountain\\hall_bottom_2.png");
            //PixelGrid::SetImage(552, 512, texture);
        }




#pragma endregion

#pragma region Player
        std::shared_ptr<Shader> animationShader = ResourceManager::Find<Shader>(L"SpriteAnimationShader");
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(animationShader);
        material->SetRenderingMode(eRenderingMode::Transparent);
        ResourceManager::Insert(L"m_SpriteAnimation", material);
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

        std::shared_ptr<Shader> shader = ResourceManager::Find<Shader>(L"DebugShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"SpriteShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"SpriteAnimationShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        D3D11_INPUT_ELEMENT_DESC particleLayout[1] = {};

        particleLayout[0].AlignedByteOffset = 0;
        particleLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        particleLayout[0].InputSlot = 0;
        particleLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        particleLayout[0].SemanticName = "POSITION";
        particleLayout[0].SemanticIndex = 0;

        shader = ResourceManager::Find<Shader>(L"ParticleShader");
        GetDevice()->CreateInputLayout(particleLayout, 1, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"ParticleAnimationShader");
        GetDevice()->CreateInputLayout(particleLayout, 1, shader->GetVSCode(), shader->GetInputLayoutAddressOf());


        D3D11_INPUT_ELEMENT_DESC triangleLayout[2] = {};

        triangleLayout[0].AlignedByteOffset = 0;
        triangleLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        triangleLayout[0].InputSlot = 0;
        triangleLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        triangleLayout[0].SemanticName = "POSITION";
        triangleLayout[0].SemanticIndex = 0;
       
        triangleLayout[1].AlignedByteOffset = 12;
        triangleLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        triangleLayout[1].InputSlot = 0;
        triangleLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        triangleLayout[1].SemanticName = "COLOR";
        triangleLayout[1].SemanticIndex = 0;

        shader = ResourceManager::Find<Shader>(L"TriangleShader");
        GetDevice()->CreateInputLayout(triangleLayout, 2, shader->GetVSCode(), shader->GetInputLayoutAddressOf());
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

    std::mutex debugMutex;
    void PushDebugMeshAttribute(DebugMesh mesh)
    {
        std::unique_lock lock(debugMutex);
        debugMeshs.push_back(mesh);
    }

    void Render()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"Noise01");

        texture->BindShader(eShaderStage::VS, 1);
        texture->BindShader(eShaderStage::HS, 1);
        texture->BindShader(eShaderStage::DS, 1);
        texture->BindShader(eShaderStage::GS, 1);
        texture->BindShader(eShaderStage::PS, 1);
        texture->BindShader(eShaderStage::CS, 1);

        ConstantBuffer* cb = constantBuffer[(UINT)eCBType::Noise];
        NoiseCB data = {};
        data.textureSize.x = texture->GetImageWidth();
        data.textureSize.y = texture->GetImageHeight();

        cb->SetBufferData(&data);
        cb->BindConstantBuffer(eShaderStage::VS);
        cb->BindConstantBuffer(eShaderStage::GS);
        cb->BindConstantBuffer(eShaderStage::PS);
        cb->BindConstantBuffer(eShaderStage::CS);

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

    void LoadUIResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        std::shared_ptr<Texture> inventory_box = ResourceManager::Load<Texture>(L"inventory_box", L"..\\Resources\\Texture\\Inventory\\full_inventory_box.png");   
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(inventory_box);
        ResourceManager::Insert(L"m_inventory_box", material);

        std::shared_ptr<Texture> inventory_box_highlihgt = ResourceManager::Load<Texture>(L"inventory_box_highlihgt", L"..\\Resources\\Texture\\Inventory\\full_inventory_box_highlight.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(inventory_box_highlihgt);
        ResourceManager::Insert(L"m_inventory_box_highlihgt", material);

        std::shared_ptr<Texture> mouse_cursor = ResourceManager::Load<Texture>(L"mouse_cursor", L"..\\Resources\\Texture\\Mouse\\mouse_cursor.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(mouse_cursor);
        ResourceManager::Insert(L"m_mouse_cursor", material);

        std::shared_ptr<Texture> highlight = ResourceManager::Load<Texture>(L"highlight", L"..\\Resources\\Texture\\Inventory\\highlight.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(highlight);
        ResourceManager::Insert(L"m_highlight", material);

        std::shared_ptr<Texture> info_box = ResourceManager::Load<Texture>(L"info_box", L"..\\Resources\\Texture\\Inventory\\info_box.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(info_box);
        ResourceManager::Insert(L"m_info_box", material);
    }

    void LoadWandTextureResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        std::shared_ptr<Texture> BlastWand_0585 = ResourceManager::Load<Texture>(L"BlastWand_0585", L"..\\Resources\\Texture\\Wand\\BlastWand_0585.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(BlastWand_0585);
        ResourceManager::Insert(L"m_BlastWand_0585", material);

        std::shared_ptr<Texture> BoltWand_0997 = ResourceManager::Load<Texture>(L"BoltWand_0997", L"..\\Resources\\Texture\\Wand\\BoltWand_0997.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(BoltWand_0997);
        ResourceManager::Insert(L"m_BoltWand_0997", material);
    }

    void LoadMonsterResource()
    {
        ResourceManager::Load<Texture>(L"shotgunner_weak", L"..\\Resources\\Texture\\Monster\\shotgunner_weak.png");
    }

    void LoadSpellResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        ResourceManager::Load<Texture>(L"SparkBolt", L"..\\Resources\\Texture\\Spell\\SparkBolt\\spark.png");
        ResourceManager::Load<Texture>(L"Explosion_SparkBolt", L"..\\Resources\\Texture\\Spell\\SparkBolt\\explosion_008_pink.png");
        ResourceManager::Load<Texture>(L"Muzzle_SparkBolt", L"..\\Resources\\Texture\\Spell\\SparkBolt\\muzzle_large_pink.png");

        std::shared_ptr<Shader> ParticleShader = ResourceManager::Find<Shader>(L"ParticleShader");
        material = std::make_shared<Material>();
        material->SetShader(ParticleShader);
        ResourceManager::Insert(L"m_Particle", material);

        std::shared_ptr<Texture> Particle_Pink = ResourceManager::Load<Texture>(L"Particle_Pink", L"..\\Resources\\Texture\\Spell\\SparkBolt\\plasma_fading_pink.png");
        material = std::make_shared<Material>();
        material->SetShader(ParticleShader);
        material->SetTexture(Particle_Pink);
        ResourceManager::Insert(L"m_Particle_Pink", material);

        std::shared_ptr<Texture> light_bullet = ResourceManager::Load<Texture>(L"light_bullet", L"..\\Resources\\Texture\\Spell\\SparkBolt\\light_bullet.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(light_bullet);
        ResourceManager::Insert(L"m_light_bullet", light_bullet);
    }

    void LoadEffectResource()
    {
        ResourceManager::Load<Texture>(L"explosion_128", L"..\\Resources\\Texture\\Effect\\explosion_128.png");
        ResourceManager::Load<Texture>(L"explosion_128_poof", L"..\\Resources\\Texture\\Effect\\explosion_128_poof.png");
    }

    void LoadParticleResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Shader> ParticleAnimationShader = ResourceManager::Find<Shader>(L"ParticleAnimationShader");
        std::shared_ptr<Material> material;

        ResourceManager::Load<Texture>(L"Noise01", L"..\\Resources\\Texture\\noise_01.png");

        std::shared_ptr<Texture> fire_falling = ResourceManager::Load<Texture>(L"fire_falling", L"..\\Resources\\Texture\\Particle\\fire_falling.png");
        material = std::make_shared<Material>();
        material->SetShader(ParticleAnimationShader);
        material->SetTexture(fire_falling);
        ResourceManager::Insert(L"m_fire_falling", material);

        std::shared_ptr<Texture> smoke_orange = ResourceManager::Load<Texture>(L"smoke_orange", L"..\\Resources\\Texture\\Particle\\smoke_orange.png");
        material = std::make_shared<Material>();
        material->SetShader(ParticleAnimationShader);
        material->SetTexture(smoke_orange);
        ResourceManager::Insert(L"m_smoke_orange", material);
    }
}



