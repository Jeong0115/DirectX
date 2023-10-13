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
    };

}