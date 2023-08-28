#pragma once

#include "zzComponent.h"

namespace zz
{
    class Transform;
    class Collider : public Component
    {
    public:
        Collider();
        virtual ~Collider();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        Vector3 GetScale() { return mScale; }
        void SetScale(Vector3 scale) { mScale = scale; }
        void SetScale(float x, float y, float z) { mScale = Vector3(x, y, z); }

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
