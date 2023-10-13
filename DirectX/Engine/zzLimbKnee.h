#pragma once

#include "zzGameObject.h"

namespace zz
{
    class LimbKnee : public GameObject
    {
    public:
        LimbKnee();
        virtual ~LimbKnee();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;
    };
}

