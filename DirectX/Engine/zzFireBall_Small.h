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

        void SetDetectPos(Vector3 pos);
        void OnCollision(struct Element& element);

    private:
        virtual void dead() override;;

        class RigidBody* mRigid;
    };
}

