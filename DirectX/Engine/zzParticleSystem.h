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

    private:
        StructedBuffer* mBuffer;
        StructedBuffer* mSharedBuffer;

        std::shared_ptr<ParticleShader> mCS;

        UINT    mCount;
        Vector4 mStartSize;
        Vector4 mEndSize;

        float   mLifeTime;
        float   mTime;
    };
}
