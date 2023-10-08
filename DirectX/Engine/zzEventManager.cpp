#include "zzEventManager.h"
#include "zzSceneManager.h"
#include "zzScene.h"

namespace zz
{
    std::vector<EventInfo> EventManager::mEventsInfo = {};
    std::vector<GameObject*> EventManager::mDeactivateList = {};
    std::vector<std::vector<std::function<void(const EvenetData&)>>> EventManager::mEventListeners((UINT)eEvent::End);
    std::vector<EvenetData> EventManager::mEventDatas = {};
    std::vector<std::function<void()>> EventManager::mOtherEvents = {};

    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    void EventManager::Initialize()
    {
        mEventListeners.resize((UINT)eEvent::End);
    }

    void EventManager::Update()
    {
        for (UINT i = 0; i < mEventDatas.size(); i++)
        {
            for (UINT j = 0; j < mEventListeners[(UINT)mEventDatas[i].eventType].size(); j++)
            {
                mEventListeners[(UINT)mEventDatas[i].eventType][j](mEventDatas[i]);
            }
        }
        mEventDatas.clear();

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

        for (int i = 0; i < mOtherEvents.size(); i++)
        {
            mOtherEvents[i]();
        }
        mOtherEvents.clear();
    }

    void EventManager::RegisterEvent(EvenetData data)
    {
        mEventDatas.push_back(data);
    }

    void EventManager::RegisterListener(eEvent eventType, std::function<void(const EvenetData& data)> listener)
    {
        mEventListeners[(UINT)eventType].push_back(listener);
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