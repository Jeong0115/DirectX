#include "zzRedTeleport.h"
#include "zzApplication.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzCollider.h"
#include "zzAnimator.h"

namespace zz
{
    RedTeleport::RedTeleport()
        : mConnectStage(L"RewardRoom")
    {
    }
    RedTeleport::~RedTeleport()
    {
    }
    void RedTeleport::Initialize()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"red_portal");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"pp", texture, Vector2(0.0f, 0.0f), Vector2(35.0f, 35.0f), 8, Vector2::Zero, 0.05f);
        animator->PlayAnimation(L"pp", true);

        GetComponent<Transform>()->SetScale(35.f, 35.f, 1.0f);
        AddComponent<Collider>()->SetScale(20.f, 20.f, 1.0f);

        //Light* light = AddComponent<Light>();
        //light->SetLightScale(110.f, 110, 1.0f);
        //light->SetLightColor(10 / 255.f, 10 / 255.f, 10 / 255.f, 0.5f);

        //mParticle = AddComponent<ParticleSystem>();
        //mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        //mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        //mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCircleCS"));
        //
        //Particle particles[1500] = {};
        //mParticle->CreateStructedBuffer(sizeof(Particle), 1500, eViewType::UAV, particles, true, 0, 14, 0);
        //mParticle->CreateStructedBuffer(sizeof(ParticleCircleShared), 1, eViewType::UAV, nullptr, true, 6, 14, 1);
        //
        //mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        //mShareData.color_min = mShareData.color_max = Vector4(117.f / 255.f, 65.f / 255.f, 129.f / 255.f, 0.06f);
        //
        //mShareData.curPosition = GetComponent<Transform>()->GetPosition() + 0.0f;
        //mShareData.randPositionMax = Vector2(0.0f, 0.0f);
        //mShareData.randPositionMin = Vector2(0.0f, 0.0f);
        //mShareData.maxParticleCnt = 115.f;
        //mShareData.particleCnt = 115.f;
        //mShareData.create = false;
        //mShareData.lightScale = Vector4(10.0f, 10.0f, 1.0f, 0.0f);
        //mShareData.randSpeed = Vector2(2.0f, 8.0f);
        //mShareData.radius = 15.f;
        //mShareData.randLifeTime = Vector2(2.0f, 4.0f);
        //
        //MeshRenderer* particleLight = new MeshRenderer();
        //particleLight->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        //particleLight->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        //mParticle->SetParticleLight(particleLight);

        GameObject::Initialize();
    }
    void RedTeleport::Update()
    {
        GameObject::Update();
    }
    void RedTeleport::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void RedTeleport::Render()
    {
        GameObject::Render();
    }

    void RedTeleport::OnCollisionEnter(GameObject* other)
    {
        if (other->GetLayerType() == eLayerType::Player)
        {
            Application::LoadNextScene(mConnectStage);
        }
    }
    void RedTeleport::OnCollisionStay(GameObject* other)
    {
    }
    void RedTeleport::OnCollisionExit(GameObject* other)
    {
    }
}