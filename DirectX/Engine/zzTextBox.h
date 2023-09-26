#pragma once

#include "zzUI.h"

namespace zz
{
    class TextBox : public UI
    {
    public:
        TextBox(const std::wstring& text, Vector3 scale);
        virtual ~TextBox();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

    private:
        class Texture* mText;
    };
}

