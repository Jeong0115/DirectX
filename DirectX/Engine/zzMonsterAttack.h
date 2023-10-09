#pragma once

#include "zzGameObject.h"

namespace zz
{
    class MonsterAttack : public GameObject
    {
    public:
        MonsterAttack();
        virtual ~MonsterAttack();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void OnCollisionEnter(GameObject* other);
        virtual void OnCollisionStay(GameObject* other);
        virtual void OnCollisionExit(GameObject* other);

    protected:
        float mDamage;
        float mTime;
        float mLimitTime;
    };
}


