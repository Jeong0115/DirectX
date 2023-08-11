#pragma once

#include "zzUI.h"

namespace zz
{
    class InfoBoxTexture : public UI
    {
    public:
        InfoBoxTexture(eUIType type);
        virtual ~InfoBoxTexture();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void CreateItemTexture(std::wstring material_name, Vector3 scale);

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;
    };
}

