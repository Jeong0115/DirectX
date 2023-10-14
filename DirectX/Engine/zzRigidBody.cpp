#include "zzRigidBody.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzGameObject.h"

namespace zz
{
    RigidBody::RigidBody()
        : Component(eComponentType::RigidBody)
        , mVelocity(Vector3::Zero)
        , mAirFriction(0.f)
        , mGravity(400.f)
        , mTransform(nullptr)
        , mbGround(false)
        , mbRotate(false)
    {
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::Initialize()
    {    
        mTransform = GetOwner()->GetComponent<Transform>();
    }

    void RigidBody::Update()
    {
        Vector3 pos = mTransform->GetPosition();

        if (mbOrbitalMotion)
        {
            OrbitalMotion();
        }

        if (!mbGround)
        {
            mVelocity.y -= mGravity * (float)Time::DeltaTime();
        }

        if (mAirFriction != 0.f)
        {
            mVelocity.x *= pow((1.0f / mAirFriction), (float)Time::DeltaTime());
        }

        pos += mVelocity * (float)Time::DeltaTime();

        if (mbRotate)
        {
            Vector3 normalize = mVelocity;
            normalize.Normalize();

            float angle = atan2(normalize.y, normalize.x);

            mTransform->SetRotationZ(angle);
        }


        mTransform->SetPosition(pos);
    }

    void RigidBody::LateUpdate()
    {
    }

    void RigidBody::Render()
    {
    }

    void RigidBody::SetStartVelocity(float speed, float angle)
    {
        mVelocity.x = cos(angle) * speed;
        mVelocity.y = sin(angle) * speed;
    }

    void RigidBody::SetStartVelocity(float speed, Vector3 direction)
    {
        mVelocity.x = direction.x * speed;
        mVelocity.y = direction.y * speed;
    }

    void RigidBody::OrbitalMotion()
    {
        Vector3 pos = mTransform->GetPosition();
        Vector3 toCenter = mOrbitalCenter - pos;
        
        float speed = sqrt(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y);

        Vector3 perpendicular = Vector3(-toCenter.y, toCenter.x, 0.0f);
        perpendicular.Normalize();

        mVelocity = perpendicular * speed;

    }

    void RigidBody::SetGround(bool isGround)
    {
        mbGround = isGround;

        if (isGround)
        {
            mVelocity.y = 0.0f;
        }   
    }
    
    Vector3 RigidBody::GetPredictedPosition()
    {
        Vector3 pos = mTransform->GetPosition();

        pos += mVelocity * (float)Time::DeltaTime();

        return pos;
    }
}