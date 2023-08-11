#include "zzParticleSystem.h"
#include "zzTime.h"
#include "zzResource.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzGraphics.h"

namespace zz
{
    
    ParticleSystem::ParticleSystem()
        : mBuffer(nullptr)
        , mSharedBuffer(nullptr)
        , mCS(nullptr)
        , mCount(0)
        , mStartSize(Vector4::One)
        , mEndSize(Vector4::One)
        , mLifeTime(0.f)
        , mTime(0.f)
        , mPrevPos(Vector4::Zero)
        , mIndex(0)
    {
        std::shared_ptr<Mesh> mesh = ResourceManager::Find<Mesh>(L"PointMesh");
        SetMesh(mesh);

        std::shared_ptr<Material> material = ResourceManager::Find<Material>(L"ParticleMaterial");
        SetMaterial(material);

        mCS = ResourceManager::Find<ParticleShader>(L"ParticleSystemShader");

        Particle particles[500] = {};

        mBuffer = new StructedBuffer();
        mBuffer->Create(sizeof(Particle), 500, eViewType::UAV, particles, true);

        mSharedBuffer = new StructedBuffer();
        mSharedBuffer->Create(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true);
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::Initialize()
    {
        Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
        mPrevPos = Vector4(pos.x, pos.y, pos.z, 0.0f);
    }

    void ParticleSystem::Update()
    {
    }

    void ParticleSystem::LateUpdate()
    {
        Vector3 curPos = GetOwner()->GetComponent<Transform>()->GetPosition();

        ParticleShared shareData = {};
        shareData.curPosition = Vector4((int)curPos.x, (int)curPos.y, (int)curPos.z, 0.0f);
        shareData.distance = shareData.curPosition - mPrevPos;
        shareData.distance.z = 0;
        shareData.angle = GetOwner()->GetComponent<Transform>()->GetRotation().z;
        mPrevPos = shareData.curPosition;

        UINT count = (UINT)max(fabs(shareData.distance.x), fabs(shareData.distance.y));
        shareData.SetActiveCount = count;
        shareData.RemainingActiveCount = count;
        shareData.index = mIndex;
        mIndex += count;

        mSharedBuffer->SetData(&shareData, 1);

        mCS->SetParticleBuffer(mBuffer);
        mCS->SetSharedBuffer(mSharedBuffer);
        mCS->OnExcute();
    }

    void ParticleSystem::Render()
    {
        GetOwner()->GetComponent<Transform>()->BindConstantBuffer();

        mBuffer->BindSRV(eShaderStage::VS, 14);
        mBuffer->BindSRV(eShaderStage::GS, 14);
        mBuffer->BindSRV(eShaderStage::PS, 14);

        GetMesh()->BindBuffer();
        GetMaterial()->Binds();
        GetMesh()->RenderInstanced(500);

        mBuffer->Clear();
    }
}