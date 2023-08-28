#include "zzCollider.h"
#include "zzGameObject.h"
#include "zzTransform.h"

namespace zz
{
    UINT Collider::mColliderNumber = 0;

    Collider::Collider()
        : Component(eComponentType::Collider)
        , mTransform(nullptr)
        , mColliderID(mColliderNumber++)
        , mScale(Vector3::Zero)
    {
    }

    Collider::~Collider()
    {
    }

    void Collider::Initialize()
    {
        mTransform = GetOwner()->GetComponent<Transform>();

        if(mScale == Vector3::Zero)
        {
            mScale = mTransform->GetScale();
        }
    }

    void Collider::Update()
    {
    }

    void Collider::LateUpdate()
    {
    }

    void Collider::Render()
    {
    }

    void Collider::OnCollisionEnter(Collider* other)
    {
        GetOwner()->OnCollisionEnter(other->GetOwner());
    }

    void Collider::OnCollisionStay(Collider* other)
    {
        GetOwner()->OnCollisionStay(other->GetOwner());
    }

    void Collider::OnCollisionExit(Collider* other)
    {
        GetOwner()->OnCollisionExit(other->GetOwner());
    }
}