#include "zzSmoke_Orange.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzMesh.h"
#include "zzAnimator.h"
#include "zzRigidBody.h"
#include "zzTransform.h"
#include "zzTime.h"
#include "zzObjectPoolManager.h"
#include "zzParticleSystem.h"
#include "zzLight.h"

namespace zz
{   
    Smoke_Orange::Smoke_Orange()
        : mTime(0.f)
        , mParticle(nullptr)
        , mPrevPos(Vector3::Zero)
    {
        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"smoke_orange");

        MeshRenderer* meshRender = AddComponent<MeshRenderer>();
        meshRender->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        meshRender->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"smoke_orange_play", texture, Vector2(0.0f, 0.0f), Vector2(16.f, 16.f), 8, Vector2::Zero, 0.03f);
        animator->PlayAnimation(L"smoke_orange_play", false);

        RigidBody* rigid = AddComponent<RigidBody>();
        rigid->SetGravity(90.f);
        rigid->SetAirFirction(2.f);

        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        AddComponent<Light>()->SetLightScale(16.f, 16.f, 1.0f);

        // 나중에 색 건들자
        GetComponent<Light>()->SetLightColor(1.0f,0.5f, 0.0f, 1.0f);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ProjectileParticleCS"));

        Particle particles[15] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 15, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ProjectileShared), 1, eViewType::UAV, nullptr, true, 1, 14, 1);
    }

    Smoke_Orange::~Smoke_Orange()
    {
    }

    void Smoke_Orange::Initialize()
    {
        GetComponent<RigidBody>()->SetStartVelocity(randi(200) + 50.f, randf(6.283185f));
        GetComponent<Animator>()->PlayAnimation(L"smoke_orange_play", false);

        GameObject::Initialize();
    }

    void Smoke_Orange::Update()
    {
        mTime += (float)Time::DeltaTime();
        mPrevPos = GetComponent<Transform>()->GetPosition();

        if (mTime >= 3.0f)
        {
            mTime = 0.0f;
            ObjectPoolManager::ReturnObjectToPool<Smoke_Orange>(this);
        }
        GameObject::Update();
    }

    void Smoke_Orange::LateUpdate()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        ProjectileShared shareData = {};
        shareData.curPosition = Vector4((int)curPos.x, (int)curPos.y, (int)curPos.z, 0.0f);
        shareData.distance = shareData.curPosition - mPrevPos;
        shareData.distance.z = 0;
        shareData.color = Vector4(255.f / 255.f, 96.f / 255.f, 0.f / 255.f, 127.f / 255.f);
        shareData.angle = GetComponent<Transform>()->GetRotation().z;

        UINT count = (UINT)std::max(fabs(shareData.distance.x), fabs(shareData.distance.y));
        shareData.activeCount = count;
        shareData.totalActiveCount = count;
        //mIndex += count;
        mParticle->SetStructedBufferData(&shareData, 1, 1);

        GameObject::LateUpdate();
    }

    void Smoke_Orange::Render()
    {
        GameObject::Render();
    }

    void Smoke_Orange::SetPosition(Vector3 pos)
    {
        GetComponent<Transform>()->SetPosition(pos);
        mPrevPos = pos;
    }
}