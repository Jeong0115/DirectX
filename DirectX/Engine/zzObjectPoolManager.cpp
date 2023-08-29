#include "zzObjectPoolManager.h"
#include "zzGameObject.h"
#include "zzSmoke_Orange.h"

namespace zz
{
    std::map<std::string, std::stack<GameObject*>> ObjectPoolManager::mObjectPool = {};

    ObjectPoolManager::ObjectPoolManager()
    {
    }

    ObjectPoolManager::~ObjectPoolManager()
    {
    }

    void ObjectPoolManager::Initialize()
    {
        std::stack<GameObject*> smoke;
        for (int i = 0; i < 48; i++)
        {
            Smoke_Orange* object = new Smoke_Orange();
            SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::Object);
            smoke.push(object);
            object->SetActive(false);
        }

        mObjectPool.insert(std::make_pair(typeid(Smoke_Orange).name(), smoke));
    }

    void ObjectPoolManager::Release()
    {
        for (auto& iter : mObjectPool)
        {
            while (!iter.second.empty())
            {
                iter.second.top() = nullptr;
                iter.second.pop();
            }
        }
        mObjectPool = {};
    }

  
}