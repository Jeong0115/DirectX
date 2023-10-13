#pragma once

#include "zzMonsterAttack.h"

namespace zz
{
    class FireBall_Small : public MonsterAttack
    {
    public:
        FireBall_Small();
        virtual ~FireBall_Small();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other) override;
        virtual void OnCollisionStay(GameObject* other)  override;
        virtual void OnCollisionExit(GameObject* other)  override;

        void SetDetectPos(Vector3 pos);
        void OnCollision(struct Element& element);

    private:
        class RigidBody* mRigid;
    };
}

