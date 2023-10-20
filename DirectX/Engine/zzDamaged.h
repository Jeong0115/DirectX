#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Damaged : public GameObject
    {
    public:
        Damaged();
        virtual ~Damaged();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void OnEvent(const struct EvenetData& data);

    private:
        Vector4 mColor;
        float mTime;
    };
}
