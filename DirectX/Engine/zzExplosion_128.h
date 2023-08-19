#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Explosion_128 : public GameObject
    {
    public:
        Explosion_128();
        ~Explosion_128();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void EndAnimation();
    private:
        bool mbFirstFrame;
    };
}
