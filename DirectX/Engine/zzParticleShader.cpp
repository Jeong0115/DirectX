#include "zzParticleShader.h"
#include "zzConstantBuffer.h"
#include "zzRenderer.h"
#include "zzTime.h"

namespace zz
{
    ParticleShader::ParticleShader()
        : ComputeShader(128, 1, 1)
        , mParticleBuffer(nullptr)
        , mSharedBuffer(nullptr)
    {
    }

    ParticleShader::~ParticleShader()
    {
    }

    void ParticleShader::Binds()
    {
        mParticleBuffer->BindUAV();
        mSharedBuffer->BindUAV();

        mGroupX = mParticleBuffer->GetStride() / mThreadGroupCountX + 1;
        mGroupY = 1;
        mGroupZ = 1;
    }

    void ParticleShader::Clear()
    {
        mParticleBuffer->Clear();
        mSharedBuffer->Clear();
    }

    void ParticleShader::SetParticleBuffer(StructedBuffer* particleBuffer)
    {
        mParticleBuffer = particleBuffer;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Particle];

        renderer::ParticleCB data = {};
        data.elementCount = mParticleBuffer->GetStride();
        data.deltaTime = (float)Time::DeltaTime();

        cb->SetBufferData(&data);
        cb->BindConstantBuffer(eShaderStage::CS);
    }
}