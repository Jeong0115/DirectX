#pragma once

#include "zzGameObject.h"

namespace zz
{
    class MonsterBody : public GameObject
    {
    public:
        MonsterBody();
        virtual ~MonsterBody();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void Impulse(Vector2 impulse, Vector2 point = Vector2::Zero) { mImpulse = impulse; mPoint = point; }
        void SetTexture(std::shared_ptr<class Texture> tex);
        void SetBox2dScale(Vector3 scale) { mBoxScale = scale; }

    private:
        Vector2 mImpulse;
        Vector2 mPoint;
        Vector3 mBoxScale;

        float   mTime;
    };
}

