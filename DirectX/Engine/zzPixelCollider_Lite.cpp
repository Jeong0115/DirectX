#include "zzPixelCollider_Lite.h"
#include "zzPixelWorld.h"
#include "zzTransform.h"
#include "zzGameObject.h"

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
}