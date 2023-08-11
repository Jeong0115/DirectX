#include "zzLayer.h"

namespace zz
{
    Layer::Layer()
    {
    }

    Layer::~Layer()
    {
        for (GameObject* gameObject : mGameObjects)
        {
            if (gameObject == nullptr)
                continue;

            delete gameObject;
            gameObject = nullptr;
        }
    }

    void Layer::Initialize()
    {
        for (GameObject* gameObject : mGameObjects)
        {
            gameObject->Initialize();
        }
    }

    void Layer::Update()
    {
        for (GameObject* gameObject : mGameObjects)
        {
            gameObject->Update();
        }
    }

    void Layer::LateUpdate()
    {
        for (GameObject* gameObject : mGameObjects)
        {
            gameObject->LateUpdate();
        }
    }

    void Layer::Render()
    {
        std::vector<GameObject*>::iterator iter = mGameObjects.begin();

        for (; iter != mGameObjects.end(); )
        {
            if ((*iter)->IsDead())
            {
                iter = mGameObjects.erase(iter);
            }
            else
            {
                (*iter)->Render();
                iter++;
            }
        }
    }

    void Layer::AddGameObject(GameObject* gameObject)
    {
        mGameObjects.push_back(gameObject);
    }
}