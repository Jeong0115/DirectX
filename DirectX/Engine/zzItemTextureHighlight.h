#pragma once

#include "zzUI.h"

namespace zz
{
    class ItemTextureHighlight : public UI
    {
    public:
        ItemTextureHighlight(eUIType type);
        virtual ~ItemTextureHighlight();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;
    };
}

