#pragma once

#include "zzGameObject.h"

namespace zz
{
    class Equipment : public GameObject
    {
    public:
        Equipment();
        virtual ~Equipment();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void UseEquipment();

        Vector3 GetInitialPos() { return mInitialPosition; }

    protected:
        Vector3 mInitialPosition;
    };
}
