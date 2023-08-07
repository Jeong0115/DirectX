#pragma once

#include "zzUI.h"

namespace zz
{
    class Mouse : public UI
    {
    public:
        Mouse(eUIType type);
        virtual ~Mouse();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

    private:
        class ItemTexture* mControllUI;
    };
}

