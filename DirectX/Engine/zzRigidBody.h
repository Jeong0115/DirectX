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
        void SetVelocity(Vector3 vel)       { mVelocity = vel; }
        void SetVelocityX(float speedX)     { if (!mbImpulse) mVelocity.x = speedX; }
        void SetVelocityY(float speedY)     { if (!mbImpulse) mVelocity.y = speedY; }
        void ApplyResistance(float resistance) { mVelocity *= resistance; }

        void Impulse(Vector3 vel, float time) { mVelocity = vel; mImpulseTime = time; mbImpulse = true; }
        void OrbitalMotion();

        void SetOrbitalMotion(bool isOrbitalMotion, Vector3 center = Vector3::Zero) { mbOrbitalMotion = isOrbitalMotion; mOrbitalCenter = center; }
        void SetGround(bool isGround);
        int GetDirection() { return mVelocity.x >= 0 ? 1 : -1; }

        void SetRotate(bool isRotate) { mbRotate = isRotate; }
        bool IsGround() { return mbGround; }

        Vector3 GetPredictedPosition();
        Vector3 GetVelocity() { return mVelocity; }

        float GetVelocityY() { return mVelocity.y; }

    private:
        class Transform* mTransform;

        Vector3 mVelocity;
        Vector3 mOrbitalCenter;

        float   mAirFriction;
        float   mGravity;
        float   mImpulseTime;
            
        bool    mbImpulse;
        bool    mbGround;
        bool    mbRotate;
        bool    mbOrbitalMotion;
    };
}

