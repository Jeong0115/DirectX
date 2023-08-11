#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Spell : public GameObject
    {
    public:
        Spell();
        ~Spell();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();
    };
}
