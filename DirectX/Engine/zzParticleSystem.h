#pragma once

#include "zzMeshRenderer.h"
#include "zzStructedBuffer.h"
#include "zzParticleShader.h"

namespace zz
{
    class ParticleSystem : public MeshRenderer
    {
    public:
        ParticleSystem();
        ~ParticleSystem();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void CreateStructedBuffer(UINT size, UINT stride, eViewType type, void* data, bool isStaging, UINT uavSlot, int tempType);
        void SetStructedBufferData(void* data, UINT bufferCount, int tempType);
        void SetParticleShader(std::shared_ptr<ParticleShader> shader) { mCS = shader; }

    private:
        StructedBuffer* mBuffer;
        StructedBuffer* mSharedBuffer;

        std::shared_ptr<ParticleShader> mCS;

        UINT    mBufferSlot;

    };
}
