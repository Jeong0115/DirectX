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

namespace zz
{
    SparkBolt::SparkBolt()
        : mPrevPos(Vector4::Zero)
    {
        int a = 0;
    }

    SparkBolt::~SparkBolt()
    {
        int a = 0;
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

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"ParticleMaterial"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleSystemShader"));

        Particle particles[500] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 500, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 1, 14, 1);

        GameObject::Initialize();
    }

    void SparkBolt::Update()
    {
        mT += Time::DeltaTime();

        Transform* tr = GetComponent<Transform>();
        Vector3 prevPos = tr->GetPosition();
        mPrevPos = Vector4(prevPos.x, prevPos.y, prevPos.z, 0.0f);
        Vector3 curPos;
        curPos = prevPos + mDirection * 700.f * (float)Time::DeltaTime();

        tr->SetPosition(curPos);

        if (mT >= 2.5f)
        {
            DeleteObject(this, eLayerType::Object);
        }
        GameObject::Update();
    }

    void SparkBolt::LateUpdate()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        ParticleShared shareData = {};
        shareData.curPosition = Vector4((int)curPos.x, (int)curPos.y, (int)curPos.z, 0.0f);
        shareData.distance = shareData.curPosition - mPrevPos;
        shareData.distance.z = 0;
        shareData.angle = GetComponent<Transform>()->GetRotation().z;
        mPrevPos = shareData.curPosition;

        UINT count = (UINT)std::max(fabs(shareData.distance.x), fabs(shareData.distance.y));
        shareData.SetActiveCount = count;
        shareData.RemainingActiveCount = count;
        shareData.index = mIndex;
        mIndex += count;
        mParticle->SetStructedBufferData(&shareData, 1, 1);

        GameObject::LateUpdate();
    }

    void SparkBolt::Render()
    {
        GameObject::Render();
    }
}