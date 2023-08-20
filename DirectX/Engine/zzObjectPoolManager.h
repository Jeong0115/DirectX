#pragma once

#include "zzEngine.h"
#include "zzGameObject.h"
#include "zzSceneManager.h"

namespace zz
{
    class ObjectPoolManager
    {
    public:
        ObjectPoolManager();
        ~ObjectPoolManager();

        static void Initialize();
        static void Release();

        template <typename T>
        static T* GetObjectInPool()
        {
            auto iter = mObjectPool.find(typeid(T).name());

            if (iter != mObjectPool.end())
            {
                if (iter->second.empty())
                {
                    T* object = new T();
                    SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::Object);
                    object->Initialize();
                    object->SetActive(true);
                    return object;
                }

                T* object = static_cast<T*>(iter->second.top());
                object->Initialize();
                object->SetActive(true);
                iter->second.pop();
                return object;
            }

            return nullptr;
        }

        template <typename T>
        static void ReturnObjectToPool(T* object)
        {
            auto iter = mObjectPool.find(typeid(T).name());

            if (iter != mObjectPool.end())
            {
                iter->second.push(object);
            }
            else
            {
                std::stack<GameObject*> stack;
                stack.push(object);

                mObjectPool.insert(std::make_pair(typeid(T).name(), stack));
            }
            object->SetActive(false);
        }   

    private:
        static std::map<std::string, std::stack<GameObject*>> mObjectPool;
    };
}

