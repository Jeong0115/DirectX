#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Animator;
    class ExplosionEffect : public GameObject
    {
    public:
        ExplosionEffect();
        virtual ~ExplosionEffect();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetAnimator(Animator* ani, const std::wstring name);

    private:
        void endAnimation();

    private:
        Animator* mAnimator;

    };
}

