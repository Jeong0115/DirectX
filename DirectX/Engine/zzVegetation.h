#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Vegetation : public GameObject
    {
    public:
        Vegetation();
        virtual ~Vegetation();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void OnCollision(struct Element& element);

    private:
        float   mTime;
        float   mAlpha;
        bool    mbDead;
    };
}

