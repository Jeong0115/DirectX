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

        void SetPosition(Vector3 pos);
        void SetPosition(float x, float y, float z);
            
        void SetScale(Vector3 scale);
        void SetScale(float x, float y, float z);

        void KeepObjectEndAnimatiom(bool bKeep) { mbDelete = !bKeep; }
        void Play();
        
    private:
        void endAnimation();

    private:
        std::wstring mAnimationName;

        Animator* mAnimator;
        bool mbDelete;
    };
}

