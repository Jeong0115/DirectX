#include "zzScene.h"

namespace zz
{
    Scene::Scene()
    {
        mLayers.resize((UINT)eLayerType::End);
    }

    Scene::~Scene()
    {
    }

    void Scene::Initialize()
    {
        for (Layer& layer : mLayers)
        {
            layer.Initialize();
        }
    }

    void Scene::Update()
    {
        for (Layer& layer : mLayers)
        {
            layer.Update();
        }
    }

    void Scene::LateUpdate()
    {
        for (Layer& layer : mLayers)
        {
            layer.LateUpdate();
        }
    }

    void Scene::Render()
    {
        for (Layer& layer : mLayers)
        {
            layer.Render();
        }
    }

    void Scene::MovePlayer()
    {
    }

    void Scene::AddGameObject(GameObject* gameObject, eLayerType type)
    {
        gameObject->SetLayerType(type);
        mLayers[(UINT)type].AddGameObject(gameObject);
    }
}