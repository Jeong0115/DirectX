#pragma once

#include "zzMonsterAttack.h"

namespace zz
{
    class OrbPink : public MonsterAttack
    {
    public:
        OrbPink();
        virtual ~OrbPink();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

    private:
        virtual void dead() override;

        ExplosionEffect* mEffect;
    };
}

