#include "zzParticleSystem.h"
#include "zzTime.h"
#include "zzResource.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzGraphics.h"
#include "zzApplication.h"
namespace zz
{
    
    ParticleSystem::ParticleSystem()
        : mBuffer(nullptr)
        , mSharedBuffer(nullptr)
        , mCS(nullptr)
        , mBufferSlot(0)
        , mbAcitve(true)
        , mbLight(false)
    {
    }

    ParticleSystem::~ParticleSystem()
    {
        delete mBuffer;
        delete mSharedBuffer;

        if (mbLight)
        {
            delete lightRenderer;
        }
    }

    void ParticleSystem::Initialize()
    {
    }

    void ParticleSystem::Update()
    {
    }

    void ParticleSystem::LateUpdate()
    {
        if (!mbAcitve) return;

        mCS->SetParticleBuffer(mBuffer);
        mCS->SetSharedBuffer(mSharedBuffer);
        mCS->OnExcute();

        if (mbLight)
        {
            if (Application::LightDisabled) return;
            mBuffer->BindSRV(eShaderStage::VS);
            mBuffer->BindSRV(eShaderStage::GS);
            mBuffer->BindSRV(eShaderStage::PS);

            lightRenderer->GetMesh()->BindBuffer();
            lightRenderer->GetMaterial()->Binds();
            lightRenderer->GetMesh()->RenderInstanced(mBuffer->GetStride());

            mBuffer->Clear();
        }
    }

    void ParticleSystem::Render()
    {
        if (!mbAcitve) return;

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