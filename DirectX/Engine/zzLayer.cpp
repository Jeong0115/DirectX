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
        int c = 0;
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
            if (gameObject->GetActive())
            {
                gameObject->Update();
            }
        }
    }

    void Layer::LateUpdate()
    {
        for (GameObject* gameObject : mGameObjects)
        {
            if (gameObject->GetActive())
            {
                gameObject->LateUpdate();
            }
        }
    }

    void Layer::Render()
    {
        std::vector<GameObject*>::iterator iter = mGameObjects.begin();

        for (; iter != mGameObjects.end(); )
        {
            if ((*iter)->IsAllowDelete())
            {
                iter = mGameObjects.erase(iter);
            }
            else if ((*iter)->GetActive())
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