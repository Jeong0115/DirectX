#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Smoke_Orange : public GameObject
    {
    public:
        Smoke_Orange();
        virtual ~Smoke_Orange();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

    private:
        float mTime;
    };
}

