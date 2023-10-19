#include "zzPixelCollider_Lite.h"
#include "zzPixelWorld.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzRigidBody.h"

namespace zz
{
    PixelCollider_Lite::PixelCollider_Lite()
        : Component(eComponentType::PixelCollider_Lite)
        , mOffset(Vector3::Zero)
        , mTransform(nullptr)
        , mEvent(nullptr)
    {
    }

    PixelCollider_Lite::~PixelCollider_Lite()
    {
    }

    void PixelCollider_Lite::Initialize()
    {
        mTransform = GetOwner()->GetComponent<Transform>();
    }

    void PixelCollider_Lite::Update()
    {
        Vector3 ownerPos = mTransform->GetPosition() + mOffset;

        Element& element = PixelWorld::GetElement(ownerPos.x, -ownerPos.y);
        mEvent(element);
    }

    void PixelCollider_Lite::LateUpdate()
    {

    }

    void PixelCollider_Lite::Render()
    {
    }

    void PixelCollider_Lite::SetPositionPrevCollision()
    {
        Vector3 vel = GetOwner()->GetComponent<RigidBody>()->GetVelocity();
        vel *= -1;
        
        Vector3 pos = mTransform->GetPosition();
        
        int dx = vel.x;
        int dy = vel.y;
        
        int steps = std::max(std::abs(dx), std::abs(dy));
        
        float xIncrement = dx / (float)steps;
        float yIncrement = dy / (float)steps;
        
        float x = pos.x;
        float y = pos.y;
        
        for (int i = 0; i < steps + 1; i++)
        {
            if (PixelWorld::GetElement(x, -y).Type != eElementType::SOLID)
            {
                mTransform->SetPosition(x, y, pos.z);
                return;
            }
            x += xIncrement;
            y += yIncrement;
        }
        mTransform->SetPosition(x, y, pos.z);
    }
}