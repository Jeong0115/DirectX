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

    class EventManager
    {
    public:
        EventManager();
        virtual ~EventManager();

    public:
        static void Update();

        static void AddEvent(const EventInfo event)
        {
            mEventsInfo.push_back(event);
        }


    private:
        static void Execute(const EventInfo event);

    private:
        static std::vector<EventInfo> mEventsInfo;
        static std::vector<GameObject*> mDeactivateList;
    };
}
