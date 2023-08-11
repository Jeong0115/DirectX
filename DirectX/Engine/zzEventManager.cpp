#include "zzEventManager.h"
#include "zzSceneManager.h"
#include "zzScene.h"

namespace zz
{
    std::vector<EventInfo> EventManager::mEventsInfo = {};
    std::vector<GameObject*> EventManager::mDeactivateList = {};

    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    void EventManager::Update()
    {
        for (UINT i = 0; i < mDeactivateList.size(); i++)
        {
            delete mDeactivateList[i];
        }
        mDeactivateList.clear();
        for (UINT i = 0; i < mEventsInfo.size(); i++)
        {
            Execute(mEventsInfo[i]);
        }
        mEventsInfo.clear();
    }

    void EventManager::Execute(const EventInfo event)
    {
        switch (event.eventType)
        {
        case eEventType::Create_Object:
        {
            GameObject* obj = (GameObject*)event.objPointer;
            eLayerType type = (eLayerType)event.objLayer;

            obj->Initialize();

            SceneManager::GetActiveScene()->AddGameObject(obj, type);
        }
        break;
        case eEventType::Delete_Object:
        {
            GameObject* obj = (GameObject*)event.objPointer;
            obj->SetState(GameObject::eState::Dead);
            mDeactivateList.push_back(obj);
        }
        break;



        default:
            break;
        }
    }
}