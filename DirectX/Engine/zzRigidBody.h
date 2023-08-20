#pragma once

#include "zzComponent.h"

namespace zz
{
    class RigidBody : public Component
    {
    public:
        RigidBody();
        virtual ~RigidBody();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetStartVelocity(float speed, float angle);
        void SetAirFirction(float friction) { mAirFriction = friction; }
        void SetGravity(float gravity) { mGravity = gravity; }

    private:
        class Transform* mTransform;

        Vector3 mVelocity;
        float   mAirFriction;
        float   mGravity;

    };
}

