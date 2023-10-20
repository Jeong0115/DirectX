#include "zzBox2dCollider.h"
#include "zzBox2dWorld.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzPixelWorld.h"

namespace zz
{
    Box2dCollider::Box2dCollider()
        : Component(eComponentType::Box2dCollider)
        , mBody(nullptr)
        , mTransform(nullptr)
    {
    }

    Box2dCollider::~Box2dCollider()
    {
        if (mBody != nullptr)
        {
            Box2dWorld::DeleteBody(mBody);
            mBody = nullptr;
        }
    }

    void Box2dCollider::Initialize()
    {
        mTransform = GetOwner()->GetComponent<Transform>();

        //b2Vec2 position = mBody->GetPosition();
        //float angle = mBody->GetAngle();

        //mTransform->SetPositionXY(position.x, -position.y);
        //mTransform->SetRotationZ(-angle);
    }

    void Box2dCollider::Update()
    {
        if (mBody != nullptr)
        {
            b2Vec2 position = mBody->GetPosition();
            float angle = mBody->GetAngle();

            mTransform->SetPositionXY(position.x, -position.y);
            mTransform->SetRotationZ(-angle);
        }

    }

    void Box2dCollider::LateUpdate()
    {
        if (mBody != nullptr)
        {
            b2Vec2 position = mBody->GetPosition();
            PixelWorld::RenewalChunkBody(position.x, position.y);
        }
    }

    void Box2dCollider::Render()
    {
    }

    void Box2dCollider::Create(Vector3 pos, Vector3 scale)
    {
        Box2dWorld::CreateBody(mBody, eBodyShape::Box, Vector2(scale.x, scale.y), pos);
    }

    void Box2dCollider::Release()
    {
        if (mBody != nullptr)
        {
            Box2dWorld::DeleteBody(mBody);
            mBody = nullptr;
        }
    }

    void Box2dCollider::ApplyLinearImpulse(Vector2 impulse, Vector2 point)
    {
        b2Vec2 position = mBody->GetWorldCenter();
        position.x += point.x;
        position.y += point.y;

        mBody->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), position, true);
    }

}