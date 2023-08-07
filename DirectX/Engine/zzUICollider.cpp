#include "zzUICollider.h"
#include "zzGameObject.h"
#include "zzUI.h"

namespace zz
{
    UINT UICollider::mColliderNumber = 0;

    UICollider::UICollider()
        : Component(eComponentType::UICollider)
        , mTransform(nullptr)
        , mColliderID(mColliderNumber++)
        , mScale(Vector3::Zero)
    {
    }

    UICollider::~UICollider()
    {
    }

    void UICollider::Initialize()
    {
        mTransform = GetOwner()->GetComponent<Transform>();

        if(mScale == Vector3::Zero)
        {
            mScale = mTransform->GetScale();
        }
    }

    void UICollider::Update()
    {
    }

    void UICollider::LateUpdate()
    {
    }

    void UICollider::Render()
    {
    }

    void UICollider::OnCollisionEnter(UICollider* other)
    {
        dynamic_cast<UI*>(GetOwner())->OnCollisionEnter(other->GetOwner());
    }

    void UICollider::OnCollisionStay(UICollider* other)
    {
        dynamic_cast<UI*>(GetOwner())->OnCollisionStay(other->GetOwner());
    }

    void UICollider::OnCollisionExit(UICollider* other)
    {
        dynamic_cast<UI*>(GetOwner())->OnCollisionExit(other->GetOwner());
    }
}