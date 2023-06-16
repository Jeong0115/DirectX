#pragma once

#include "zzEntity.h"

namespace zz
{
    using namespace zz::math;

    class GameObject;
    class Component : public Entity
    {
    public:
        Component(eComponentType type);
        virtual ~Component();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        GameObject* GetOwner() { return mOwner; }
        void SetOwner(GameObject* owner) { mOwner = owner; }

    private:
        const eComponentType mType;
        GameObject* mOwner;
    };
}

