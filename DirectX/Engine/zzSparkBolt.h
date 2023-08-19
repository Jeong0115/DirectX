#pragma once

#include "zzAttackSpell.h"

namespace zz
{
    class SparkBolt : public AttackSpell
    {
    public:
        SparkBolt();
        ~SparkBolt();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

    private:
        class ParticleSystem* particle;

        double mT = 0.0;
        float c = 0.0f;
        Vector4 mPrevPos;
        UINT mIndex = 0;
    };
}
