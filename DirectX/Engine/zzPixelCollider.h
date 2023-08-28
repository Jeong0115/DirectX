#pragma once

#include "zzComponent.h"

namespace zz
{
    class Transform;
    class RigidBody;
    class PixelCollider : public Component
    {
    public:
        PixelCollider();
        virtual ~PixelCollider();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetCollision(Vector3 offset, Vector3 scale) { mCollisionOffset = offset; mCollisionScale = scale; }
        void SetClimbY(float y) { mClimbOverY = y; }

    private:
        void checkSidePixel(Vector3 collisionPos, Vector3 velocity);
        bool checkFloorPixel(Vector3 predictedPos);
        void preventTunneling(Vector3 collisionPos);

    private:
        Transform* mTransform;
        RigidBody* mRigid;

        Vector3 mCollisionOffset;
        Vector3 mCollisionScale;

        float mClimbOverY;
    };
}

