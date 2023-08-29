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
        void SetStartVelocity(float speed, Vector3 direction);

        void SetAirFirction(float friction) { mAirFriction = friction; }
        void SetGravity(float gravity)      { mGravity = gravity; }
        void SetVelocityX(float speedX)     { mVelocity.x = speedX; }
        void SetVelocityY(float speedY)     { mVelocity.y = speedY; }
        void ApplyResistance(float resistance) { mVelocity *= resistance; }

        void SetGround(bool isGround);

        void SetRotate(bool isRotate) { mbRotate = isRotate; }
        bool IsGround() { return mbGround; }

        Vector3 GetPredictedPosition();
        Vector3 GetVelocity() { return mVelocity; }

    private:
        class Transform* mTransform;

        Vector3 mVelocity;
        float   mAirFriction;
        float   mGravity;

        bool    mbGround;
        bool    mbRotate;
    };
}

