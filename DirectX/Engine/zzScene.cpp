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

    void Scene::AddGameObject(eLayerType type, GameObject* gameObject)
    {
        mLayers[(UINT)type].AddGameObject(gameObject);
    }
}