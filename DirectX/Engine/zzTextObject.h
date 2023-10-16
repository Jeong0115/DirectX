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

        void SetColor(Vector3 color) { mColor = color; }
        void SetLifeTime(float time) { mLifeTime = time; }
        void SetSpeed(float speed) { mSpeed = speed; }

        void WriteText(const std::wstring& text, Vector3 scale);
        void WriteDamage(const std::wstring& text, Vector3 scale = Vector3::One);

    private:
        class Texture* mText;

        Vector3 mColor;

        float mLifeTime;
        float mSpeed;
    };
}
