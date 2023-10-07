#pragma once

#include "zzGameObject.h"

namespace zz
{
    class ShotGunner_Weak : public GameObject
    {
    public:
        ShotGunner_Weak();
        virtual ~ShotGunner_Weak();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;


    private:
        class Animator* mAnimator;
    };
}

