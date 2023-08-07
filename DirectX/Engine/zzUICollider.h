#pragma once

#include "zzComponent.h"
#include "zzTransform.h"

namespace zz
{
    class UICollider : public Component
    {
    public:
        UICollider();
        ~UICollider();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        Vector3 GetScale() { return mScale; }
        void SetScale(Vector3 scale) { mScale = scale; }

        void OnCollisionEnter   (UICollider* other);
        void OnCollisionStay    (UICollider* other);
        void OnCollisionExit    (UICollider* other);

        Transform* GetTransform()   { return mTransform; }
        UINT GetColliderID()        { return mColliderID; }

    private:
        static UINT mColliderNumber;
        UINT        mColliderID;
        Transform*  mTransform;

        Vector3     mScale;
    };
}
