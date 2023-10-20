#pragma once

#include "zzGameObject.h"

namespace zz
{
    class BugExplosion : public GameObject
    {
    public:
        BugExplosion();
        virtual ~BugExplosion();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

        void Dead();

    private:
        float mTime = 0.f;
    };
}

