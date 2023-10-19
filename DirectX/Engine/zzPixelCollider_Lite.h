#pragma once

#include "zzComponent.h"
#include "zzElement.h"
namespace zz
{
    class PixelCollider_Lite : public Component
    {
    public:
        PixelCollider_Lite();
        virtual ~PixelCollider_Lite();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void SetOffest(float x, float y, float z) { mOffset = Vector3(x, y, z); }
        void SetOffest(Vector3 offset) { mOffset = offset; }
        void SetCollisionEvent(const std::function<void(Element& element)>& callback) { mEvent = callback; }

        void SetPositionPrevCollision();

    private:
        std::function<void(Element& element)> mEvent;
        Vector3 mOffset;

        class Transform* mTransform;
    };
}

