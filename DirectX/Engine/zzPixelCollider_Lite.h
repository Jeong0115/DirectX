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

        void SetCollisionEvent(const std::function<void(Element& element)>& callback) { mEvent = callback; }

    private:
        std::function<void(Element& element)> mEvent;

        class Transform* mTransform;
    };
}

