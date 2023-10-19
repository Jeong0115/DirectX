#pragma once

#include "zzGameObject.h"

namespace zz
{
    class RedTeleport : public GameObject
    {
    public:
        RedTeleport();
        virtual ~RedTeleport();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

    private:
        class ParticleSystem* mParticle;
        struct ParticleCircleShared mSharedData;

        std::wstring mConnectStage;

        float mTime;
    };
}
