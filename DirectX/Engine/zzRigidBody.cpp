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
        , mGravity(0.f)
        , mTransform(nullptr)
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

        pos += mVelocity * (float)Time::DeltaTime();
        mVelocity.y -= mGravity * (float)Time::DeltaTime();
        mVelocity.x *= pow((1.0f / mAirFriction), (float)Time::DeltaTime());
    

        //if (fabsf(mVelocity.x) <= 1.0f)
        //{
        //    mVelocity.x = 0.0f;
        //}

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
}