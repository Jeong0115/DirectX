#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Tail1 : public GameObject
    {
    public:
        Tail1();
        virtual ~Tail1();

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