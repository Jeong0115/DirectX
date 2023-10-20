#pragma once

#include "zzMonsterAttack.h"

namespace zz
{
    class Bug : public MonsterAttack
    {
    public:
        Bug();
        virtual ~Bug();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

        virtual void dead();

    };
}

