#include "zzLayer.h"

namespace zz
{
    Layer::Layer()
    {
    }

    Layer::~Layer()
    {
    }

    void Layer::Initialize()
    {
        for (GameObject* gameObj : mGameObjects)
        {
            gameObj->Initialize();
        }
    }

    void Layer::Update()
    {
        for (GameObject* gameObj : mGameObjects)
        {
            gameObj->Update();
        }
    }

    void Layer::LateUpdate()
    {
        for (GameObject* gameObj : mGameObjects)
        {
            gameObj->LateUpdate();
        }
    }

    void Layer::Render()
    {
        for (GameObject* gameObj : mGameObjects)
        {
            gameObj->Render();
        }
    }

    void Layer::AddGameObject(GameObject* gameObject)
    {
        mGameObjects.push_back(gameObject);
    }
}