#pragma once

#include "zzComponent.h"
#include "../External/Box2d/include/box2d.h"

namespace zz
{
    class Box2dCollider : public Component
    {
    public:
        Box2dCollider();
        virtual ~Box2dCollider();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void Create(Vector3 pos, Vector3 scale);
        void Release();

        void ApplyLinearImpulse(Vector2 impulse, Vector2 point = Vector2::Zero);

    private:
        b2Body* mBody;
        class Transform* mTransform;
    };
}