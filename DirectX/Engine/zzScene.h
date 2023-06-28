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
        std::vector<Layer> mLayers; // �����ͷ� ���� �׳� ���� ������ �غ���
    };
}
