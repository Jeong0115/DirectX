#pragma once

#include "zzUI.h"
#include "zzInfoBoxTexture.h"

namespace zz
{
    class InfoBox : public UI
    {
    public:
        InfoBox(eUIType type);
        virtual ~InfoBox();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void LinkWand(class Wand* wand, InfoBoxTexture* texture);

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

    private:
        InfoBoxTexture* mWandtexture;
        class Wand* mWand;
    };
}
