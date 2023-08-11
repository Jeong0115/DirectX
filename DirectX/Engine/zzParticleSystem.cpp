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
    {
        std::shared_ptr<Mesh> mesh = ResourceManager::Find<Mesh>(L"PointMesh");
        SetMesh(mesh);

        std::shared_ptr<Material> material = ResourceManager::Find<Material>(L"ParticleMaterial");
        SetMaterial(material);

        mCS = ResourceManager::Find<ParticleShader>(L"ParticleSystemShader");

        Particle particles[1000] = {};
        for (size_t i = 0; i < 1000; i++)
        {
            Vector4 pos = Vector4::Zero;
            pos.x += 00 ;    
            pos.y += 00 ;

            int sign = rand() % 2;
            if (sign == 0)
                pos.x *= -1.0f;
            sign = rand() % 2;
            if (sign == 0)
                pos.y *= -1.0f;

            particles[i].velocity =
                Vector4(cosf((float)i * (XM_2PI / (float)1000))
                    , sinf((float)i * (XM_2PI / 1000.f))
                    , 0.0f, 1.0f);

            particles[i].position = pos;
            particles[i].speed = 10.0f;
            particles[i].active = 1;
        }

        mBuffer = new StructedBuffer();
        mBuffer->Create(sizeof(Particle), 1000, eViewType::UAV, particles);

        mSharedBuffer = new StructedBuffer();
        mSharedBuffer->Create(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true);
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::Initialize()
    {
    }

    void ParticleSystem::Update()
    {
    }

    void ParticleSystem::LateUpdate()
    {
        float AliveTime = 1.0f / 1.0f;
        mTime += (float)Time::DeltaTime();

        if (mTime > AliveTime)
        {
            float f = (mTime / AliveTime);
            UINT AliveCount = (UINT)f;
            mTime = f - floor(f);

            ParticleShared shareData = {};
            shareData.sharedActiveCount = 100;
            mSharedBuffer->SetData(&shareData, 1);
        }
        else
        {
            ParticleShared shareData = {};
            shareData.sharedActiveCount = 0;
            mSharedBuffer->SetData(&shareData, 1);
        }


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

        GetMaterial()->Binds();
        GetMesh()->RenderInstanced(1000);

        mBuffer->Clear();
    }
}