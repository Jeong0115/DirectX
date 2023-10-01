#pragma once

#include "zzGameObject.h"

namespace zz
{
    class TextObject : public GameObject
    {
    public:
        TextObject();
        virtual ~TextObject();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void WriteText(const std::wstring& text, Vector3 scale);

    private:
        class Texture* mText;

        float mLifeTime;
    };
}
