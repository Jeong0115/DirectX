#include "zzRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzMaterial.h"
#include "zzParticleShader.h"
#include "zzAudioClip.h"

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

    void LoadCoalmineResource();
    void LoadUIResource();
    void LoadWandTextureResource();
    void LoadSpellResource();
    void LoadEffectResource();
    void LoadParticleResource();
    void LoadMonsterResource();
    void LoadVegetationResource();
    void LoadEtc();

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
            mesh->CreateVertexBuffer(pointVertexes.data(), static_cast<UINT>(pointVertexes.size()));
            mesh->CreateIndexBuffer(pointIndexes.data(), static_cast<UINT>(pointIndexes.size()));
            mesh->SetName(L"PointMesh");
            ResourceManager::Insert(L"PointMesh", mesh);
        }


        Vertex lightVertexes[4] = {};

        std::vector<UINT> lightIndexes = {};

        lightIndexes.push_back(0);
        lightIndexes.push_back(1);
        lightIndexes.push_back(2);

        lightIndexes.push_back(1);
        lightIndexes.push_back(3);
        lightIndexes.push_back(2);

        lightVertexes[0].pos = Vector3(-1.0f, 1.0f, 0.0f);
        lightVertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        lightVertexes[0].uv = Vector2(0.0f, 0.0f);
        
        lightVertexes[1].pos = Vector3(1.0f, 1.0f, 0.0f);
        lightVertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        lightVertexes[1].uv = Vector2(1.0f, 0.0f);
       
        lightVertexes[2].pos = Vector3(-1.0f, -1.0f, 0.0f);
        lightVertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
        lightVertexes[2].uv = Vector2(0.0f, 1.0f);
       
        lightVertexes[3].pos = Vector3(1.0f, -1.0f, 0.0f);
        lightVertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        lightVertexes[3].uv = Vector2(1.0f, 1.0f);
        
        std::shared_ptr<Mesh> lightMesh = std::make_shared<Mesh>();
        lightMesh->CreateVertexBuffer(lightVertexes, 4);
        lightMesh->CreateIndexBuffer(lightIndexes.data(), (UINT)lightIndexes.size());
        ResourceManager::Insert(L"LightMesh", lightMesh);

        constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
        constantBuffer[(UINT)eCBType::Transform]->CreateConstantBuffer(sizeof(TransformCB));

        constantBuffer[(UINT)eCBType::Color] = new ConstantBuffer(eCBType::Color);
        constantBuffer[(UINT)eCBType::Color]->CreateConstantBuffer(sizeof(ColorCB));

        constantBuffer[(UINT)eCBType::Slider] = new ConstantBuffer(eCBType::Slider);
        constantBuffer[(UINT)eCBType::Slider]->CreateConstantBuffer(sizeof(SliderCB));

        constantBuffer[(UINT)eCBType::Animator] = new ConstantBuffer(eCBType::Animator);
        constantBuffer[(UINT)eCBType::Animator]->CreateConstantBuffer(sizeof(AnimatorCB));

        constantBuffer[(UINT)eCBType::Flip] = new ConstantBuffer(eCBType::Flip);
        constantBuffer[(UINT)eCBType::Flip]->CreateConstantBuffer(sizeof(FlipCB));

        constantBuffer[(UINT)eCBType::Particle] = new ConstantBuffer(eCBType::Particle);
        constantBuffer[(UINT)eCBType::Particle]->CreateConstantBuffer(sizeof(ParticleCB));

        constantBuffer[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
        constantBuffer[(UINT)eCBType::Noise]->CreateConstantBuffer(sizeof(NoiseCB));

        constantBuffer[(UINT)eCBType::ColorRange] = new ConstantBuffer(eCBType::ColorRange);
        constantBuffer[(UINT)eCBType::ColorRange]->CreateConstantBuffer(sizeof(ColorRangeCB));
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

        std::shared_ptr<Shader> sliderShader = std::make_shared<Shader>();
        sliderShader->CreateShader(eShaderStage::VS, L"SpriteVS.hlsl", "main");
        sliderShader->CreateShader(eShaderStage::PS, L"SliderPS.hlsl", "main");
        ResourceManager::Insert(L"SliderShader", sliderShader);

		std::shared_ptr<Shader> textShader = std::make_shared<Shader>();
		textShader->CreateShader(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		textShader->CreateShader(eShaderStage::PS, L"TextPS.hlsl", "main");
		ResourceManager::Insert(L"TextShader", textShader);

        std::shared_ptr<Shader> fadeSpriteShader = std::make_shared<Shader>();
        fadeSpriteShader->CreateShader(eShaderStage::VS, L"SpriteVS.hlsl", "main");
        fadeSpriteShader->CreateShader(eShaderStage::PS, L"FadeSpritePS.hlsl", "main");
        ResourceManager::Insert(L"FadeSpriteShader", fadeSpriteShader);

        std::shared_ptr<Shader> lightShader = std::make_shared<Shader>();
        lightShader->CreateShader(eShaderStage::VS, L"LightVS.hlsl", "main");
        lightShader->CreateShader(eShaderStage::PS, L"LightPS.hlsl", "main");
        lightShader->SetBSState(eBSType::LightMap);
        ResourceManager::Insert(L"LightShader", lightShader);

        std::shared_ptr<Shader> lightMapShader = std::make_shared<Shader>();
        lightMapShader->CreateShader(eShaderStage::VS, L"LightMapVS.hlsl", "main");
        lightMapShader->CreateShader(eShaderStage::PS, L"LightMapPS.hlsl", "main");
        lightMapShader->SetBSState(eBSType::LightMap);
        ResourceManager::Insert(L"LightMapShader", lightMapShader);

        std::shared_ptr<Shader> lightMapShader2 = std::make_shared<Shader>();
        lightMapShader2->CreateShader(eShaderStage::VS, L"LightMapVS.hlsl", "main");
        lightMapShader2->CreateShader(eShaderStage::PS, L"BloomPS.hlsl", "main");
        lightMapShader2->SetBSState(eBSType::AddLIght);
        ResourceManager::Insert(L"BloomShader", lightMapShader2);

        std::shared_ptr<Shader> lightMapShadert = std::make_shared<Shader>();
        lightMapShadert->CreateShader(eShaderStage::VS, L"LightMapVS.hlsl", "main");
        lightMapShadert->CreateShader(eShaderStage::PS, L"AddDarkPS.hlsl", "main");
        lightMapShadert->SetBSState(eBSType::AddDark);
        ResourceManager::Insert(L"BloomShaderT", lightMapShadert);

        std::shared_ptr<Shader> spriteAnimationShader = std::make_shared<Shader>();
        spriteAnimationShader->CreateShader(eShaderStage::VS, L"SpriteAnimationVS.hlsl", "main");
        spriteAnimationShader->CreateShader(eShaderStage::PS, L"SpriteAnimationPS.hlsl", "main");
        ResourceManager::Insert(L"SpriteAnimationShader", spriteAnimationShader);

        std::shared_ptr<Shader> fadeAnimationShader = std::make_shared<Shader>();
        fadeAnimationShader->CreateShader(eShaderStage::VS, L"SpriteAnimationVS.hlsl", "main");
        fadeAnimationShader->CreateShader(eShaderStage::PS, L"FadeAnimationPS.hlsl", "main");
        ResourceManager::Insert(L"FadeAnimationShader", fadeAnimationShader);

        std::shared_ptr<ParticleShader> psSystemShader = std::make_shared<ParticleShader>();
        psSystemShader->Create(L"ProjectileParticleCS.hlsl", "main");
        ResourceManager::Insert(L"ProjectileParticleCS", psSystemShader);

        std::shared_ptr<ParticleShader> psSystemShader1 = std::make_shared<ParticleShader>();
        psSystemShader1->Create(L"ParticleCS.hlsl", "main");
        ResourceManager::Insert(L"ParticleCS", psSystemShader1);

        std::shared_ptr<ParticleShader> psSystemShader2 = std::make_shared<ParticleShader>();
        psSystemShader2->Create(L"SineFuncParticleCS.hlsl", "main");
        ResourceManager::Insert(L"SineFuncParticleCS", psSystemShader2);

        std::shared_ptr<ParticleShader> psSystemShader3 = std::make_shared<ParticleShader>();
        psSystemShader3->Create(L"ParticleCircleCS.hlsl", "main");
        ResourceManager::Insert(L"ParticleCircleCS", psSystemShader3);

        std::shared_ptr<ParticleShader> psSystemShader4 = std::make_shared<ParticleShader>();
        psSystemShader4->Create(L"ParticleMakeCircleCS.hlsl", "main");
        ResourceManager::Insert(L"ParticleMakeCircleCS", psSystemShader4);

        std::shared_ptr<ComputeShader> bloom1 = std::make_shared<ComputeShader>();
        bloom1->Create(L"BloomExtractionCS.hlsl", "main");
        ResourceManager::Insert(L"BloomExtractionCS", bloom1);

        std::shared_ptr<ComputeShader> bloom2 = std::make_shared<ComputeShader>();
        bloom2->Create(L"BloomHorizonCS.hlsl", "main");
        ResourceManager::Insert(L"BloomHorizonCS", bloom2);

        std::shared_ptr<ComputeShader> bloom3 = std::make_shared<ComputeShader>();
        bloom3->Create(L"BloomVerticalCS.hlsl", "main");
        ResourceManager::Insert(L"BloomVerticalCS", bloom3);

        std::shared_ptr<ParticleShader> particleImageCS = std::make_shared<ParticleShader>();
        particleImageCS->Create(L"ParticleImageCS.hlsl", "main");
        ResourceManager::Insert(L"ParticleImageCS", particleImageCS);

        std::shared_ptr<Shader> paritcleShader = std::make_shared<Shader>();
        paritcleShader->CreateShader(eShaderStage::VS, L"ParticleVS.hlsl", "main");
        paritcleShader->CreateShader(eShaderStage::GS, L"ParticleGS.hlsl", "main");
        paritcleShader->CreateShader(eShaderStage::PS, L"ParticlePS.hlsl", "main");
        paritcleShader->SetRSState(eRSType::SolidNone);
        paritcleShader->SetDSState(eDSType::NoWrite);
        paritcleShader->SetBSState(eBSType::LightMap);
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


        std::shared_ptr<Shader> particleLightShader = std::make_shared<Shader>();
        particleLightShader->CreateShader(eShaderStage::VS, L"ParticleLightVS.hlsl", "main");
        particleLightShader->CreateShader(eShaderStage::GS, L"ParticleLightGS.hlsl", "main");
        particleLightShader->CreateShader(eShaderStage::PS, L"ParticleLightPS.hlsl", "main");
        particleLightShader->SetBSState(eBSType::LightMap);
        particleLightShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
        ResourceManager::Insert(L"ParticleLightShader", particleLightShader);

        std::shared_ptr<Shader> paritcleCirceShader = std::make_shared<Shader>();
        paritcleCirceShader->CreateShader(eShaderStage::VS, L"ParticleMakeCircleVS.hlsl", "main");
        paritcleCirceShader->CreateShader(eShaderStage::GS, L"ParticleMakeCircleGS.hlsl", "main");
        paritcleCirceShader->CreateShader(eShaderStage::PS, L"ParticlePS.hlsl", "main");
        paritcleCirceShader->SetRSState(eRSType::SolidNone);
        paritcleCirceShader->SetDSState(eDSType::NoWrite);
        paritcleCirceShader->SetBSState(eBSType::LightMap);
        paritcleCirceShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
        ResourceManager::Insert(L"ParticleCirceShader", paritcleCirceShader);

        std::shared_ptr<Shader> ParticleShader2 = ResourceManager::Find<Shader>(L"ParticleCirceShader");
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(ParticleShader2);
        ResourceManager::Insert(L"m_ParticleCirceShader", material);

    }
    void LoadResource()
    {
        LoadCoalmineResource();
        LoadParticleResource();
        LoadUIResource();
        LoadWandTextureResource();
        LoadEffectResource();
        LoadSpellResource();
        LoadMonsterResource();
        LoadVegetationResource();
        LoadEtc();

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
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"boss_arena_background", L"..\\Resources\\Texture\\BossArena\\boss_arena_background.png");
            std::shared_ptr<Material> material = std::make_shared<Material>();

            material = std::make_shared<Material>();
            material->SetShader(spriteShader);
            material->SetTexture(texture);
            ResourceManager::Insert(L"m_boss_arena_background", material);
        }

        {
            std::shared_ptr<Shader> debugShader = ResourceManager::Find<Shader>(L"DebugShader");
            std::shared_ptr<Material> material = std::make_shared<Material>();

            material = std::make_shared<Material>();
            material->SetShader(debugShader);
            ResourceManager::Insert(L"DebugMaterial", material);
        }
        {
            std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"background_coalmine", L"..\\Resources\\Texture\\Coalmine\\background_coalmine.png");

            std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
            spriteMateiral2->SetRenderingMode(eRenderingMode::Opaque);
            spriteMateiral2->SetShader(spriteShader);
            spriteMateiral2->SetTexture(texture);
            ResourceManager::Insert(L"m_background_coalmine", spriteMateiral2);
        }
		

		std::shared_ptr<Material> pixelTexture = std::make_shared<Material>();
		pixelTexture->SetShader(spriteShader);
		ResourceManager::Insert(L"m_PixelTexture", pixelTexture);

        std::shared_ptr<Shader> animationShader = ResourceManager::Find<Shader>(L"SpriteAnimationShader");
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(animationShader);
        material->SetRenderingMode(eRenderingMode::Transparent);
        ResourceManager::Insert(L"m_SpriteAnimation", material);

        std::shared_ptr<Shader> animationShader2 = ResourceManager::Find<Shader>(L"SpriteAnimationShader");
        material = std::make_shared<Material>();
        material->SetShader(animationShader2);
        material->SetRenderingMode(eRenderingMode::Opaque);
        ResourceManager::Insert(L"m_SpriteAnimation_Opaque", material);

        std::shared_ptr<Shader> fadeAnimationShader = ResourceManager::Find<Shader>(L"FadeAnimationShader");
        material = std::make_shared<Material>();
        material->SetShader(fadeAnimationShader);
        material->SetRenderingMode(eRenderingMode::Transparent);
        ResourceManager::Insert(L"m_FadeAnimation", material);
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

        shader = ResourceManager::Find<Shader>(L"SliderShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"FadeSpriteShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"LightShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

		shader = ResourceManager::Find<Shader>(L"TextShader");
		GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"LightMapShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"BloomShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"BloomShaderT");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"SpriteAnimationShader");
        GetDevice()->CreateInputLayout(arrLayout, 3, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"FadeAnimationShader");
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

        shader = ResourceManager::Find<Shader>(L"ParticleCirceShader");
        GetDevice()->CreateInputLayout(particleLayout, 1, shader->GetVSCode(), shader->GetInputLayoutAddressOf());

        shader = ResourceManager::Find<Shader>(L"ParticleLightShader");
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
        GetDevice()->BindSamplerState(eShaderStage::CS, 0, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        GetDevice()->CreateSamplerState(&desc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
        GetDevice()->BindSamplerState(eShaderStage::PS, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
        GetDevice()->BindSamplerState(eShaderStage::CS, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
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

        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;         // Use the source color
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;       // Use the destination color
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;      // Add source and destination
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;   // Use the source alpha
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;  // Use the destination alpha
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Add source and destination alpha
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        GetDevice()->CreateBlendState(&blendDesc, blendStates[(UINT)eBSType::LightMap].GetAddressOf());

        D3D11_BLEND_DESC sblendDesc = {};
        sblendDesc.RenderTarget[0].BlendEnable = true;
        sblendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;     // Use the source color (original image)
        sblendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;         // Ignore destination color (light map)
        sblendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;         // Add source and destination (though destination is ignored due to D3D11_BLEND_ZERO)
        sblendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;      // Use the source alpha
        sblendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;    // Ignore destination alpha
        sblendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;    // Add source and destination alpha (though destination is ignored due to D3D11_BLEND_ZERO)
        sblendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        GetDevice()->CreateBlendState(&blendDesc, blendStates[(UINT)eBSType::AddLIght].GetAddressOf());


        D3D11_BLEND_DESC blendDesc2 = {};
        blendDesc2.RenderTarget[0].BlendEnable = true;
        blendDesc2.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;     // Multiply by the destination color (target color)
        blendDesc2.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;         // Ignore source color (since we're multiplying it above)
        blendDesc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;         // The addition operation, but effectively this becomes multiplication due to the blend factors
        blendDesc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA; // Multiply by the destination alpha
        blendDesc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;    // Ignore source alpha
        blendDesc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;    // The addition operation for alpha, but effectively this becomes multiplication due to the blend factors
        blendDesc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        GetDevice()->CreateBlendState(&blendDesc2, blendStates[(UINT)eBSType::AddDark].GetAddressOf());
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
        data.textureSize.x = static_cast<float>(texture->GetImageWidth());
        data.textureSize.y = static_cast<float>(texture->GetImageHeight());

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

    void LoadCoalmineResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        std::shared_ptr<Texture> rock_hard_alt = ResourceManager::Load<Texture>(L"rock_hard_alt", L"..\\Resources\\Texture\\Material\\rock_hard_alt.png");
        material = std::make_shared<Material>();
        material->SetRenderingMode(eRenderingMode::Opaque);
        material->SetShader(spriteShader);
        material->SetTexture(rock_hard_alt);
        ResourceManager::Insert(L"m_rock_hard_alt", material);

        std::shared_ptr<Texture> wall_background = ResourceManager::Load<Texture>(L"wall_background", L"..\\Resources\\Texture\\Temple\\wall_background.png");
        material = std::make_shared<Material>();
        material->SetRenderingMode(eRenderingMode::Opaque);
        material->SetShader(spriteShader);
        material->SetTexture(wall_background);
        ResourceManager::Insert(L"m_wall_background", material);

        std::shared_ptr<Texture> coalpit01 = ResourceManager::Load<Texture>(L"coalpit01", L"..\\Resources\\Texture\\Coalmine\\coalpit01.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit01);
        ResourceManager::Insert(L"m_coalpit01", material);

        std::shared_ptr<Texture> coalpit01_visual = ResourceManager::Load<Texture>(L"coalpit01_visual", L"..\\Resources\\Texture\\Coalmine\\coalpit01_visual.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit01_visual);
        ResourceManager::Insert(L"m_coalpit01_visual", material);

        std::shared_ptr<Texture> coalpit02 = ResourceManager::Load<Texture>(L"coalpit02", L"..\\Resources\\Texture\\Coalmine\\coalpit02.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit02);
        ResourceManager::Insert(L"m_coalpit02", material);

        std::shared_ptr<Texture> coalpit02_visual = ResourceManager::Load<Texture>(L"coalpit02_visual", L"..\\Resources\\Texture\\Coalmine\\coalpit02_visual.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit02_visual);
        ResourceManager::Insert(L"m_coalpit02_visual", material);

        std::shared_ptr<Texture> coalpit03 = ResourceManager::Load<Texture>(L"coalpit03", L"..\\Resources\\Texture\\Coalmine\\coalpit03.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit03);
        ResourceManager::Insert(L"m_coalpit03", material);

        std::shared_ptr<Texture> coalpit03_visual = ResourceManager::Load<Texture>(L"coalpit03_visual", L"..\\Resources\\Texture\\Coalmine\\coalpit03_visual.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit03_visual);
        ResourceManager::Insert(L"m_coalpit03_visual", material);

        std::shared_ptr<Texture> coalpit04 = ResourceManager::Load<Texture>(L"coalpit04", L"..\\Resources\\Texture\\Coalmine\\coalpit04.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit04);
        ResourceManager::Insert(L"m_coalpit04", material);

        std::shared_ptr<Texture> coalpit04_visual = ResourceManager::Load<Texture>(L"coalpit04_visual", L"..\\Resources\\Texture\\Coalmine\\coalpit04_visual.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit04_visual);
        ResourceManager::Insert(L"m_coalpit04_visual", material);

        std::shared_ptr<Texture> coalpit05 = ResourceManager::Load<Texture>(L"coalpit05", L"..\\Resources\\Texture\\Coalmine\\coalpit05.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit05);
        ResourceManager::Insert(L"m_coalpit05", material);

        std::shared_ptr<Texture> coalpit05_visual = ResourceManager::Load<Texture>(L"coalpit05_visual", L"..\\Resources\\Texture\\Coalmine\\coalpit05_visual.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(coalpit05_visual);
        ResourceManager::Insert(L"m_coalpit05_visual", material);

        std::shared_ptr<Texture> carthill = ResourceManager::Load<Texture>(L"carthill", L"..\\Resources\\Texture\\Coalmine\\carthill.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(carthill);
        ResourceManager::Insert(L"m_carthill", material);
    }

    void LoadUIResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Shader> sliderShader = ResourceManager::Find<Shader>(L"SliderShader");
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

        std::shared_ptr<Texture> colors_bar_bg = ResourceManager::Load<Texture>(L"colors_bar_bg", L"..\\Resources\\Texture\\UI\\colors_bar_bg.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(colors_bar_bg);
        ResourceManager::Insert(L"m_colors_bar_bg", material);

        std::shared_ptr<Texture> colors_flying_bar = ResourceManager::Load<Texture>(L"colors_flying_bar", L"..\\Resources\\Texture\\UI\\colors_flying_bar.png");
        material = std::make_shared<Material>();
        material->SetShader(sliderShader);
        material->SetTexture(colors_flying_bar);
        ResourceManager::Insert(L"m_colors_flying_bar", material);

        std::shared_ptr<Texture> colors_health_bar = ResourceManager::Load<Texture>(L"colors_health_bar", L"..\\Resources\\Texture\\UI\\colors_health_bar.png");
        material = std::make_shared<Material>();
        material->SetShader(sliderShader);
        material->SetTexture(colors_health_bar);
        ResourceManager::Insert(L"m_colors_health_bar", material);

        std::shared_ptr<Texture> colors_health_bar_damage = ResourceManager::Load<Texture>(L"colors_health_bar_damage", L"..\\Resources\\Texture\\UI\\colors_health_bar_damage.png");
        material = std::make_shared<Material>();
        material->SetShader(sliderShader);
        material->SetTexture(colors_health_bar_damage);
        ResourceManager::Insert(L"m_colors_health_bar_damage", material);

        std::shared_ptr<Texture> colors_mana_bar = ResourceManager::Load<Texture>(L"colors_mana_bar", L"..\\Resources\\Texture\\UI\\colors_mana_bar.png");
        material = std::make_shared<Material>();
        material->SetShader(sliderShader);
        material->SetTexture(colors_mana_bar);
        ResourceManager::Insert(L"m_colors_mana_bar", material);

        std::shared_ptr<Texture> colors_reload_bar = ResourceManager::Load<Texture>(L"colors_reload_bar", L"..\\Resources\\Texture\\UI\\colors_reload_bar.png");
        material = std::make_shared<Material>();
        material->SetShader(sliderShader);
        material->SetTexture(colors_reload_bar);
        ResourceManager::Insert(L"m_colors_reload_bar", material);

        std::shared_ptr<Texture> mana = ResourceManager::Load<Texture>(L"mana", L"..\\Resources\\Texture\\UI\\mana.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(mana);
        ResourceManager::Insert(L"m_mana", material);

        std::shared_ptr<Texture> health = ResourceManager::Load<Texture>(L"health", L"..\\Resources\\Texture\\UI\\health.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(health);
        ResourceManager::Insert(L"m_health", material);

        std::shared_ptr<Texture> jetpack = ResourceManager::Load<Texture>(L"jetpack", L"..\\Resources\\Texture\\UI\\jetpack.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(jetpack);
        ResourceManager::Insert(L"m_jetpack", material);

        std::shared_ptr<Texture> reload = ResourceManager::Load<Texture>(L"reload", L"..\\Resources\\Texture\\UI\\reload.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(reload);
        ResourceManager::Insert(L"m_reload", material);

        std::shared_ptr<Texture> text_box = ResourceManager::Load<Texture>(L"text_box", L"..\\Resources\\Texture\\UI\\text_box.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(text_box);
        ResourceManager::Insert(L"m_text_box", material);

        ResourceManager::Load<Texture>(L"icon_action_type", L"..\\Resources\\Texture\\UI\\icon_action_type.png");
        ResourceManager::Load<Texture>(L"icon_damage_projectile", L"..\\Resources\\Texture\\UI\\icon_damage_projectile.png");
        ResourceManager::Load<Texture>(L"icon_fire_rate_wait", L"..\\Resources\\Texture\\UI\\icon_fire_rate_wait.png");
        ResourceManager::Load<Texture>(L"icon_mana_drain", L"..\\Resources\\Texture\\UI\\icon_mana_drain.png");
        ResourceManager::Load<Texture>(L"icon_reload_time", L"..\\Resources\\Texture\\UI\\icon_reload_time.png");
        ResourceManager::Load<Texture>(L"icon_speed_multiplier", L"..\\Resources\\Texture\\UI\\icon_speed_multiplier.png");
        ResourceManager::Load<Texture>(L"icon_spread_degrees", L"..\\Resources\\Texture\\UI\\icon_spread_degrees.png");
        ResourceManager::Load<Texture>(L"icon_gun_actions_per_round", L"..\\Resources\\Texture\\UI\\icon_gun_actions_per_round.png");
        ResourceManager::Load<Texture>(L"icon_gun_capacity", L"..\\Resources\\Texture\\UI\\icon_gun_capacity.png");
        ResourceManager::Load<Texture>(L"icon_mana_charge_speed", L"..\\Resources\\Texture\\UI\\icon_mana_charge_speed.png");
        ResourceManager::Load<Texture>(L"icon_mana_max", L"..\\Resources\\Texture\\UI\\icon_mana_max.png");
        ResourceManager::Load<Texture>(L"icon_gun_shuffle", L"..\\Resources\\Texture\\UI\\icon_gun_shuffle.png");
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

        ResourceManager::Load<Texture>(L"body", L"..\\Resources\\Texture\\Centipede\\body.png");
        ResourceManager::Load<Texture>(L"limb_long_a", L"..\\Resources\\Texture\\Centipede\\limb_long_a.png");
        ResourceManager::Load<Texture>(L"limb_long_b", L"..\\Resources\\Texture\\Centipede\\limb_long_b.png");
        ResourceManager::Load<Texture>(L"limb_long_knee", L"..\\Resources\\Texture\\Centipede\\limb_long_knee.png");
        ResourceManager::Load<Texture>(L"orb_pink_glowy", L"..\\Resources\\Texture\\Centipede\\orb_pink_glowy.png");
        ResourceManager::Load<Texture>(L"explosion_016_plasma", L"..\\Resources\\Texture\\Centipede\\explosion_016_plasma.png");
        ResourceManager::Load<Texture>(L"explosion_016_plasma_pink", L"..\\Resources\\Texture\\Centipede\\explosion_016_plasma_pink.png");
        ResourceManager::Load<Texture>(L"explosion_032_pink", L"..\\Resources\\Texture\\Centipede\\explosion_032_pink.png");
        ResourceManager::Load<Texture>(L"explosion_016", L"..\\Resources\\Texture\\Centipede\\explosion_016.png");
        ResourceManager::Load<Texture>(L"grenade_large", L"..\\Resources\\Texture\\Centipede\\grenade_large.png");
        ResourceManager::Load<Texture>(L"orb_blue_big", L"..\\Resources\\Texture\\Centipede\\orb_blue_big.png");

        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        std::shared_ptr<Texture> tail = ResourceManager::Load<Texture>(L"tail", L"..\\Resources\\Texture\\Centipede\\tail.png");
        material = std::make_shared<Material>();
        material->SetTexture(tail);
        material->SetShader(spriteShader);
        material->SetRenderingMode(eRenderingMode::Transparent);
        ResourceManager::Insert(L"m_tail", material);

        std::shared_ptr<Texture> tail_big = ResourceManager::Load<Texture>(L"tail_big", L"..\\Resources\\Texture\\Centipede\\tail_big.png");
        material = std::make_shared<Material>();
        material->SetTexture(tail);
        material->SetShader(spriteShader);
        material->SetRenderingMode(eRenderingMode::Transparent);
        ResourceManager::Insert(L"m_tail_big", material);
    }

    void LoadSpellResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        std::shared_ptr<Shader> ParticleShader = ResourceManager::Find<Shader>(L"ParticleShader");
        material = std::make_shared<Material>();
        material->SetShader(ParticleShader);
        ResourceManager::Insert(L"m_Particle", material);

        ResourceManager::Load<Texture>(L"SparkBolt", L"..\\Resources\\Texture\\Spell\\SparkBolt\\spark.png");
        ResourceManager::Load<AudioClip>(L"SparkBolt_Sound", L"..\\Resources\\Audio\\Projectiles\\spell_shoot_ver1_1.wav");

        ResourceManager::Load<Texture>(L"Explosion_SparkBolt", L"..\\Resources\\Texture\\Spell\\SparkBolt\\explosion_008_pink.png");
        ResourceManager::Load<Texture>(L"Muzzle_SparkBolt", L"..\\Resources\\Texture\\Spell\\SparkBolt\\muzzle_large_pink.png");

        std::shared_ptr<Texture> Particle_Pink = ResourceManager::Load<Texture>(L"Particle_Pink", L"..\\Resources\\Texture\\Spell\\SparkBolt\\plasma_fading_pink.png");
        material = std::make_shared<Material>();
        material->SetShader(ParticleShader);
        material->SetTexture(Particle_Pink);
        ResourceManager::Insert(L"m_Particle_Pink", material);

        std::shared_ptr<Texture> light_bullet = ResourceManager::Load<Texture>(L"light_bullet", L"..\\Resources\\Texture\\Spell\\SparkBolt\\light_bullet.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(light_bullet);
        ResourceManager::Insert(L"m_light_bullet", material);
        
        ResourceManager::Load<Texture>(L"muzzle_launcher_01", L"..\\Resources\\Texture\\Spell\\Megalaser\\muzzle_launcher_01.png");
        ResourceManager::Load<Texture>(L"wand_64", L"..\\Resources\\Texture\\Spell\\Megalaser\\wand_64.png");
        ResourceManager::Load<Texture>(L"plasma_fading_green", L"..\\Resources\\Texture\\Spell\\Megalaser\\plasma_fading_green.png");
        ResourceManager::Load<Texture>(L"explosion_016_plasma_green", L"..\\Resources\\Texture\\Spell\\Megalaser\\explosion_016_plasma_green.png");

        std::shared_ptr<Texture> megalaser_ui = ResourceManager::Load<Texture>(L"megalaser_ui", L"..\\Resources\\Texture\\Spell\\Megalaser\\megalaser.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(megalaser_ui);
        ResourceManager::Insert(L"m_megalaser_ui", material);

        material = std::make_shared<Material>();
        material->SetShader(ParticleShader);
        material->SetTexture(ResourceManager::Find<Texture>(L"plasma_fading_green"));
        ResourceManager::Insert(L"m_plasma_fading_green", material);

        ResourceManager::Load<Texture>(L"light_arrow", L"..\\Resources\\Texture\\Spell\\MagicArrow\\light_arrow.png");
        ResourceManager::Load<AudioClip>(L"MagicArrow_Sound", L"..\\Resources\\Audio\\Projectiles\\spell_shoot_ver2_1.wav");
        ResourceManager::Load<Texture>(L"explosion_016_slime", L"..\\Resources\\Texture\\Spell\\MagicArrow\\explosion_016_slime.png");
        ResourceManager::Load<Texture>(L"muzzle_laser_green_01", L"..\\Resources\\Texture\\Spell\\MagicArrow\\muzzle_laser_green_01.png");

        std::shared_ptr<Texture> spark_green = ResourceManager::Load<Texture>(L"spark_green", L"..\\Resources\\Texture\\Spell\\MagicArrow\\spark_green.png");
        material = std::make_shared<Material>();
        material->SetShader(ParticleShader);
        material->SetTexture(spark_green);
        ResourceManager::Insert(L"m_spark_green", material);

        std::shared_ptr<Texture> bullet = ResourceManager::Load<Texture>(L"bullet", L"..\\Resources\\Texture\\Spell\\MagicArrow\\bullet.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(bullet);
        ResourceManager::Insert(L"m_bullet", material);

        std::shared_ptr<Texture> water_trail = ResourceManager::Load<Texture>(L"water_trail", L"..\\Resources\\Texture\\Spell\\WaterTrail\\water_trail.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(water_trail);
        ResourceManager::Insert(L"m_water_trail", material);

        ResourceManager::Load<Texture>(L"bomb", L"..\\Resources\\Texture\\Spell\\Bomb\\bomb.png");
        ResourceManager::Load<Texture>(L"muzzle_launcher_large_01", L"..\\Resources\\Texture\\Spell\\Bomb\\muzzle_launcher_large_01.png");

        std::shared_ptr<Texture> bomb_ui = ResourceManager::Load<Texture>(L"bomb_ui", L"..\\Resources\\Texture\\Spell\\Bomb\\bomb_ui.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(bomb_ui);
        ResourceManager::Insert(L"m_bomb_ui", material);

        std::shared_ptr<Texture> divide_2 = ResourceManager::Load<Texture>(L"divide_2", L"..\\Resources\\Texture\\Spell\\Utility\\divide_2.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(divide_2);
        ResourceManager::Insert(L"m_divide_2", material);

        std::shared_ptr<Texture> divide_3 = ResourceManager::Load<Texture>(L"divide_3", L"..\\Resources\\Texture\\Spell\\Utility\\divide_3.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(divide_3);
        ResourceManager::Insert(L"m_divide_3", material);

        std::shared_ptr<Texture> divide_4 = ResourceManager::Load<Texture>(L"divide_4", L"..\\Resources\\Texture\\Spell\\Utility\\divide_4.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(divide_4);
        ResourceManager::Insert(L"m_divide_4", material);

        std::shared_ptr<Texture> divide_10 = ResourceManager::Load<Texture>(L"divide_10", L"..\\Resources\\Texture\\Spell\\Utility\\divide_10.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(divide_10);
        ResourceManager::Insert(L"m_divide_10", material);

        std::shared_ptr<Texture> speed = ResourceManager::Load<Texture>(L"speed", L"..\\Resources\\Texture\\Spell\\Utility\\speed.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(speed);
        ResourceManager::Insert(L"m_speed", material);
    }

    void LoadEffectResource()
    {
        ResourceManager::Load<Texture>(L"explosion_128", L"..\\Resources\\Texture\\Effect\\explosion_128.png");
        ResourceManager::Load<Texture>(L"explosion_128_poof", L"..\\Resources\\Texture\\Effect\\explosion_128_poof.png");

        std::shared_ptr<Shader> particleLightShader = ResourceManager::Find<Shader>(L"ParticleLightShader");
        std::shared_ptr<Shader> lightShader = ResourceManager::Find<Shader>(L"LightShader");
        std::shared_ptr<Material> material;
        
        std::shared_ptr<Texture> light_mask = ResourceManager::Load<Texture>(L"light_mask", L"..\\Resources\\Texture\\Light\\light_mask.png");
        material = std::make_shared<Material>();
        material->SetShader(lightShader);
        material->SetTexture(light_mask);
        ResourceManager::Insert(L"m_light_mask", material);

        std::shared_ptr<Texture> light_mask_smoothed = ResourceManager::Load<Texture>(L"light_mask_smoothed", L"..\\Resources\\Texture\\Light\\light_mask_smoothed.png");
        material = std::make_shared<Material>();
        material->SetShader(lightShader);
        material->SetTexture(light_mask_smoothed);
        ResourceManager::Insert(L"m_light_mask_smoothed", material);

        std::shared_ptr<Texture> particle_glow = ResourceManager::Load<Texture>(L"particle_glow", L"..\\Resources\\Texture\\Light\\particle_glow.png");
        material = std::make_shared<Material>();
        material->SetShader(lightShader);
        material->SetTexture(particle_glow);
        ResourceManager::Insert(L"m_particle_glow", material);

        std::shared_ptr<Texture> particle_glow_particleLight = ResourceManager::Load<Texture>(L"particle_glow", L"..\\Resources\\Texture\\Light\\particle_glow.png");
        material = std::make_shared<Material>();
        material->SetShader(particleLightShader);
        material->SetTexture(particle_glow_particleLight);
        ResourceManager::Insert(L"m_particle_glow_particleLight", material);

        std::shared_ptr<Texture> light_mask_linear = ResourceManager::Load<Texture>(L"light_mask_linear", L"..\\Resources\\Texture\\Light\\light_mask_linear.png");
        material = std::make_shared<Material>();
        material->SetShader(lightShader);
        material->SetTexture(light_mask_linear);
        ResourceManager::Insert(L"m_light_mask_linear", material);

        std::shared_ptr<Texture> explosion_hole = ResourceManager::Load<Texture>(L"explosion_hole", L"..\\Resources\\Texture\\Light\\explosion_hole.png");
        material = std::make_shared<Material>();
        material->SetShader(lightShader);
        material->SetTexture(explosion_hole);
        ResourceManager::Insert(L"m_explosion_hole", material);

        std::shared_ptr<Texture> explosion_stain = ResourceManager::Load<Texture>(L"explosion_stain", L"..\\Resources\\Texture\\Light\\explosion_stain.png");
        material = std::make_shared<Material>();
        material->SetShader(lightShader);
        material->SetTexture(explosion_stain);
        ResourceManager::Insert(L"m_explosion_stain", material);

        std::shared_ptr<Texture> explosion_stain_particleLight = ResourceManager::Load<Texture>(L"explosion_stain", L"..\\Resources\\Texture\\Light\\explosion_stain.png");
        material = std::make_shared<Material>();
        material->SetShader(particleLightShader);
        material->SetTexture(explosion_stain_particleLight);
        ResourceManager::Insert(L"m_explosion_stain_particleLight", material);
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

    void LoadVegetationResource()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"FadeSpriteShader");
        std::shared_ptr<Material> material;

        for (int i = 1; i <= 9; i++)
        {
            std::wstring number = std::to_wstring(i);

            std::shared_ptr<Texture> mushroom_small = ResourceManager::Load<Texture>(L"mushroom_small_0" + number, L"..\\Resources\\Texture\\Vegetation\\mushroom_small_0" + number + L".png");
            material = std::make_shared<Material>();
            material->SetShader(spriteShader);
            material->SetTexture(mushroom_small);
            ResourceManager::Insert(L"m_mushroom_small_0" + number, material);
        }
        for (int i = 1; i <= 2; i++)
        {
            std::wstring number = std::to_wstring(i);

            std::shared_ptr<Texture> shrub = ResourceManager::Load<Texture>(L"shrub_" + number, L"..\\Resources\\Texture\\Vegetation\\shrub_" + number + L".png");
            material = std::make_shared<Material>();
            material->SetShader(spriteShader);
            material->SetTexture(shrub);
            ResourceManager::Insert(L"m_shrub_" + number, material);
        }
    }

    void LoadEtc()
    {
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material;

        std::shared_ptr<Texture> teleport_center = ResourceManager::Load<Texture>(L"teleport_center", L"..\\Resources\\Texture\\Etc\\teleport_center.png");
        material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(teleport_center);
        ResourceManager::Insert(L"m_teleport_center", material);
    }
}