#pragma once

#include "zzEntity.h"
#include "zzLayer.h"

namespace zz
{
    class Scene : public Entity
    {
    public:
        Scene();
        virtual ~Scene();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        void AddGameObject(eLayerType type, GameObject* gameObject);

    private:
        std::vector<Layer> mLayers; // 포인터로 할지 그냥 할지 생각좀 해보자
    };
}

