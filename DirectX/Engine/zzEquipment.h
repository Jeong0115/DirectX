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
        virtual float GetEquipmentRate();

        Vector3 GetInitialPos() { return mInitialPosition; }
        class TextBox* GetTextBox() { return mTextBox; }

    protected:
        Vector3 mInitialPosition;
        class TextBox* mTextBox;
    };
}
