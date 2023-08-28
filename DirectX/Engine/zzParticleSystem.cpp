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
        , mBufferSlot(0)
    {
        SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));

        //{
 
        //    mCS = ResourceManager::Find<ParticleShader>(L"ParticleSystemAnimationShader");

        //    ParticleAnimation particles[500] = {};

        //    mBuffer = new StructedBuffer();
        //    mBuffer->Create(sizeof(ParticleAnimation), 500, eViewType::UAV, particles, true);
        //    mBuffer->SetUAVSlot(2);
        //    
        //    mSharedBuffer = new StructedBuffer();
        //    mSharedBuffer->Create(sizeof(ParticleAnimationShared), 1, eViewType::UAV, nullptr, true);
        //    mSharedBuffer->SetUAVSlot(3);


        //}
    }

    ParticleSystem::~ParticleSystem()
    {
        delete mBuffer;
        delete mSharedBuffer;
    }

    void ParticleSystem::Initialize()
    {
    }

    void ParticleSystem::Update()
    {
    }

    void ParticleSystem::LateUpdate()
    {

        //else if (mTempOption == 1)
        //{
        //    //ParticleAnimationShared shareData = {};
        //    //shareData.curPosition = Vector4((int)curPos.x, (int)curPos.y, (int)curPos.z, 0.0f);
        //    //shareData.gravity = Vector4(0.0f, -5.0f, 0.0f, 0.0f);
        //    //shareData.SetActiveCount = 1;
        //    //shareData.RemainingActiveCount = 1;
        //    //shareData.durtaion = 0.06f;
        //    //shareData.maxAnimationCnt = 12;

        //    //mSharedBuffer->SetData(&shareData, 1);  
        //}

        mCS->SetParticleBuffer(mBuffer);
        mCS->SetSharedBuffer(mSharedBuffer);
        mCS->OnExcute();
    }

    void ParticleSystem::Render()
    {
        //GetOwner()->GetComponent<Transform>()->BindConstantBuffer();

        mBuffer->BindSRV(eShaderStage::VS);
        mBuffer->BindSRV(eShaderStage::GS);
        mBuffer->BindSRV(eShaderStage::PS);
        
        GetMesh()->BindBuffer();
        GetMaterial()->Binds();
        GetMesh()->RenderInstanced(mBuffer->GetStride());

        mBuffer->Clear();
    }

    void ParticleSystem::CreateStructedBuffer(UINT size, UINT stride, eViewType type, void* data, bool isStaging, UINT uavSlot, UINT srvSlot, int tempType)
    {
        if(tempType == 0)
        {
            mBuffer = new StructedBuffer();
            mBuffer->Create(size, stride, eViewType::UAV, data, true);
            mBuffer->SetUAVSlot(uavSlot);
            mBuffer->SetSRVSlot(srvSlot);
        }
        else if (tempType == 1)
        {
            mSharedBuffer = new StructedBuffer();
            mSharedBuffer->Create(size, stride, eViewType::UAV, data, true);
            mSharedBuffer->SetUAVSlot(uavSlot);
        }
    }
    void ParticleSystem::SetStructedBufferData(void* data, UINT bufferCount, int tempType)
    {
        if(tempType == 0)
        {
            mBuffer->SetData(data, mBuffer->GetStride());
        }
        else if (tempType == 1)
        {
            mSharedBuffer->SetData(data, mSharedBuffer->GetStride());
        }
    }
}