#pragma once

#include "zzEngine.h"
#include "zzGameObject.h"

namespace zz
{
    struct EventInfo
    {
        eEventType	eventType;
        DWORD_PTR	objPointer;
        DWORD_PTR	objLayer;
    };

    enum class eEvent
    {
        Health_Change,
        Mana_Change,
        Enenerge_Change,
        ReCharge_Change,
        End,
    };

    struct EvenetData
    {
        eEvent eventType;
        float health;
        float mana;
        float energe;
        float recharge;
    };

    class EventManager
    {
    public:
        EventManager();
        virtual ~EventManager();

        static void Initialize();
        static void Update();
        static void AddEvent(const EventInfo event) { mEventsInfo.push_back(event); }

        static void RegisterEvent(EvenetData data);
        static void RegisterListener(eEvent eventType, std::function<void(const EvenetData&)> listener);

        static void AddOtherEvent(std::function<void()> event) { mOtherEvents.push_back(event); }
    private:
        static void Execute(const EventInfo event);

        static std::vector<EventInfo> mEventsInfo;
        static std::vector<GameObject*> mDeactivateList;

        static std::vector<std::vector<std::function<void(const EvenetData&)>>> mEventListeners;
        static std::vector<std::function<void()>> mOtherEvents;
        static std::vector<EvenetData> mEventDatas;
    };
}
