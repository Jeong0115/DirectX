#pragma once

#include "zzGameObject.h"
#include "zzExplosionEffect.h"

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

        void SetPosition(Vector3 pos);
        void SetPosition(float x, float y, float z);

        void SetDirection(Vector3 dir) { mDirection = dir; }
        void SetAngle(float angle) { mAngle = angle; }
        void SetSpeed(float speed) { mSpeed = speed; }

    protected:
        virtual void dead();

        float mDamage;
        float mTime;
        float mLimitTime;
        float mAngle;
        float mSpeed;

        Vector3 mDirection;
    };
}


