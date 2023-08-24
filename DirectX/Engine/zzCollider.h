#pragma once

#include "zzComponent.h"
#include "zzTransform.h"

namespace zz
{
    class Collider : public Component
    {
    public:
        Collider();
        ~Collider();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        Vector3 GetScale() { return mScale; }
        void SetScale(Vector3 scale) { mScale = scale; }

        void OnCollisionEnter   (Collider* other);
        void OnCollisionStay    (Collider* other);
        void OnCollisionExit    (Collider* other);

        Transform* GetTransform()   { return mTransform; }
        UINT GetColliderID()        { return mColliderID; }

    private:
        static UINT mColliderNumber;
        UINT        mColliderID;
        Transform*  mTransform;

        Vector3     mScale;
    };
}
