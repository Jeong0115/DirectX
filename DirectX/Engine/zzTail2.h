#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Tail2 : public GameObject
    {
    public:
        Tail2();
        virtual ~Tail2();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetDead() { mbDead = true; }
    private:
        bool mbDead;
        float mAngle;
        int mDir;
    };

}