#pragma once

#include "zzComponent.h"

namespace zz
{
    class Transform : public Component
    {
    public:
        Transform();
        virtual ~Transform();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void BindConstantBuffer();

        void SetPosition(Vector3 position)  { mPosition = position; }
        void SetPositionX(float x)      { mPosition.x = x; }
        void SetPositionY(float y)      { mPosition.y = y; }
        void SetPositionZ(float z)      { mPosition.z = z; }
        void SetPositionXY(float x, float y) { mPosition.x = x; mPosition.y = y; }
        void ShiftPositionY(float y)    { mPosition.y += y; }

        void SetRotation(Vector3 rotation)  { mRotation = rotation; }
        void SetRotationZ(float z) { mRotation.z = z; }

        void SetScale(Vector3 scale)        { mScale = scale; }

        void SetPosition(float x, float y, float z) { mPosition = Vector3(x, y, z); }
        void SetRotation(float x, float y, float z) { mRotation = Vector3(x, y, z); }
        void SetScale(float x, float y, float z)    { mScale = Vector3(x, y, z); }

        void SetPositionXFlip() { mPosition.x *= -1; }

        Vector3 GetPosition()   { return mPosition; }
        Vector3 GetRotation()   { return mRotation; }
        Vector3 GetScale()      { return mScale; }

        // 부모가 있을 때 사용하는 메서드
        Vector3 GetWorldPosition();
        Vector3 GetWorldRotation();

        Vector3 Foward()        { return mFoward; }
        Vector3 Right()         { return mRight; }
        Vector3 Up()            { return mUp; }

        void SetParent(Transform* transform)    { mParent = transform; }
        Transform* GetParent()                  { return mParent; }

        Matrix& GetMatrix() { return mWorld; }

    private:
        Vector3 mPosition;
        Vector3 mRotation;
        Vector3 mScale;

        Vector3 mUp;
        Vector3 mFoward;
        Vector3 mRight;

        Matrix  mWorld;

        Transform* mParent;
    };
}

