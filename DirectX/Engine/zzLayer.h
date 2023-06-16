#pragma once
#include "zzGameObject.h"
#include "zzEntity.h"

namespace zz
{
    class Layer : public Entity
    {
    public:
        Layer();
        ~Layer();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        void AddGameObject(GameObject* gameObject);

    private:
        std::vector<GameObject*> mGameObjects;
    };
}
