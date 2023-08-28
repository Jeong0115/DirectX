#include "zzSparkBolt.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzMesh.h"
#include "zzTexture.h"
#include "zzMaterial.h"
#include "zzResourceManager.h"
#include "zzTime.h"
#include "zzParticleSystem.h"
#include "zzCollider.h"
#include "zzPixelCollider_Lite.h"
#include "zzExplosionEffect.h"

namespace zz
{
    SparkBolt::SparkBolt()
        : mPrevPos(Vector4::Zero)
        , mParticle(nullptr)
        , mExplosion(nullptr)
    {
    }

    SparkBolt::~SparkBolt()
    {
    }

    void SparkBolt::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"SparkBolt");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"SparkBolt_Idle", texture, Vector2(0.0f, 1.0f), Vector2(10.0f, 10.0f), 2, Vector2::Zero, 0.2f);
        ani->PlayAnimation(L"SparkBolt_Idle", true);

        GetComponent<Transform>()->SetScale(10.f, 10.f, 1.0f);
        AddComponent<Collider>()->SetScale(10.f, 4.f, 1.0f);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle_Pink"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ProjectileParticleCS"));

        Particle particles[500] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 500, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ProjectileShared), 1, eViewType::UAV, nullptr, true, 1, 14, 1);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) {OnCollision(element); });

        GameObject::Initialize();

        mExplosion = new ExplosionEffect();
        std::shared_ptr<Texture> explosionTexture = ResourceManager::Find<Texture>(L"Explosion_SparkBolt");
        Animator* animator = new Animator();
        animator->Create(L"Explosion_SparkBolt_Play", explosionTexture, Vector2(0.0f, 1.0f), Vector2(9.0f, 9.0f), 8, Vector2::Zero, 0.01f);
        animator->PlayAnimation(L"Explosion_SparkBolt_Play", false);

        mExplosion->SetAnimator(animator, L"Explosion_SparkBolt_Play");
    }

    void SparkBolt::Update()
    {
        mT += Time::DeltaTime();

        Transform* tr = GetComponent<Transform>();
        Vector3 prevPos = tr->GetPosition();
        mPrevPos = Vector4(prevPos.x, prevPos.y, prevPos.z, 0.0f);
        Vector3 curPos;
        curPos = prevPos + mDirection * 500.f * (float)Time::DeltaTime();

        if (mT >= 2.5f && !IsDead())
        {
            DeleteObject(this, eLayerType::PlayerAttack);
        }

        tr->SetPosition(curPos);

        GameObject::Update();
    }

    void SparkBolt::LateUpdate()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        ProjectileShared shareData = {};
        shareData.curPosition = Vector4((int)curPos.x, (int)curPos.y, (int)curPos.z, 0.0f);
        shareData.distance = shareData.curPosition - mPrevPos;
        shareData.distance.z = 0;
        shareData.angle = GetComponent<Transform>()->GetRotation().z;
        mPrevPos = shareData.curPosition;

        UINT count = (UINT)std::max(fabs(shareData.distance.x), fabs(shareData.distance.y));
        shareData.activeCount = count;
        shareData.totalActiveCount = count;
        shareData.index = mIndex;
        mIndex += count;
        mParticle->SetStructedBufferData(&shareData, 1, 1);

        GameObject::LateUpdate();
    }

    void SparkBolt::Render()
    {
        GameObject::Render();
    }

    void SparkBolt::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            if (!IsDead())
            {
                DeleteObject(this, eLayerType::PlayerAttack);

                Vector3 pos = GetComponent<Transform>()->GetPosition() - (mDirection * 500.f * 0.005f);
                mExplosion->GetComponent<Transform>()->SetPosition(pos.x, pos.y, BACK_PIXEL_WORLD_Z);
                mExplosion->GetComponent<Transform>()->SetScale(9.0f, 9.0f, 1.0f);
                CreateObject(mExplosion, eLayerType::Effect);
            }
        }
    }
}