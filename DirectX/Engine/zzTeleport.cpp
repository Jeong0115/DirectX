#include "zzTeleport.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzParticleSystem.h"
#include "zzResourceManager.h"
#include "zzTime.h"
#include "zzLight.h"
#include "zzCollider.h"
#include "zzApplication.h"
#include "zzApplication.h"

namespace zz
{
    Teleport::Teleport()
        : mParticle(nullptr)
        , mShareData{}
        , mTime(0.f)
        , mConnectStage(L"Excavationsite")
    {
    }
    Teleport::~Teleport()
    {
    }

    void Teleport::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_teleport_center"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(96.f, 96.f, 1.0f);

        AddComponent<Collider>()->SetScale(30.f, 30.f, 1.0f);

        Light* light = AddComponent<Light>();
        light->SetLightScale(255.f, 255.f, 1.0f);
        light->SetLightColor(64.f / 255.f, 100.f / 255.f, 255.f / 255.f, 1.0f);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCircleCS"));

        Particle particles[1500] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 1500, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleCircleShared), 1, eViewType::UAV, nullptr, true, 6, 14, 1);

        mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShareData.color = Vector4(180.f / 255.f, 60.f / 255.f, 255.f / 255.f, 0.5f);

        mShareData.curPosition = GetComponent<Transform>()->GetPosition() + 0.0f;
        mShareData.randPositionMax = Vector2(0.0f, 0.0f);
        mShareData.randPositionMin = Vector2(0.0f, 0.0f);
        mShareData.maxParticleCnt = 115.f;
        mShareData.particleCnt = 115.f;
        mShareData.create = false;
        mShareData.randSpeed = Vector2(2.0f, 8.0f);
        mShareData.radius = 15.f;
        mShareData.randLifeTime = Vector2(2.0f, 4.0f);

        GameObject::Initialize();
    }

    void Teleport::Update()
    {
        mTime += (float)Time::DeltaTime();
        GameObject::Update();
    }

    void Teleport::LateUpdate()
    {
        if (mTime >= 0.3f)
        {
            mTime = 0.f;

            mShareData.maxParticleCnt = 115.f;
            mShareData.particleCnt = 115.f;
            mShareData.create = true;
        }
        else
        {
            mShareData.create = false;
        }
        mParticle->SetStructedBufferData(&mShareData, 1, 1);
        GameObject::LateUpdate();
    }

    void Teleport::Render()
    {
        GameObject::Render();
    }

    void Teleport::OnCollisionEnter(GameObject* other)
    {
        if (other->GetLayerType() == eLayerType::Player)
        {
            Application::LoadNextScene(mConnectStage);
        }
    }
    void Teleport::OnCollisionStay(GameObject* other)
    {
    }
    void Teleport::OnCollisionExit(GameObject* other)
    {
    }
}