#pragma once

#include "zzComputeShader.h"
#include "zzStructedBuffer.h"

namespace zz
{
    class ParticleShader : public ComputeShader
    {
    public:
        ParticleShader();
        ~ParticleShader();

        virtual void Binds() override;
        virtual void Clear() override;

        void SetParticleBuffer(StructedBuffer* particleBuffer);
        void SetSharedBuffer(StructedBuffer* sharedBuffer) { mSharedBuffer = sharedBuffer; }

    private:
        StructedBuffer* mParticleBuffer;
        StructedBuffer* mSharedBuffer;
    };
}
