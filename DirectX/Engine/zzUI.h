#pragma once

#include "zzGameObject.h"

namespace zz
{
    class UI : public GameObject
    {
    public:
        UI(eUIType type);
        virtual ~UI();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        eUIType GetUIType() { return mUIType; }

        virtual void OnCollisionEnter(GameObject* other);
        virtual void OnCollisionStay(GameObject* other);
        virtual void OnCollisionExit(GameObject* other);

    private:
        eUIType mUIType;      
    };
}

