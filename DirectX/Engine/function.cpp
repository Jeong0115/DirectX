#include "function.h"
#include "zzEventManager.h"
#include "zzGameObject.h"

namespace zz
{
    void CreateObject(GameObject* obj, eLayerType type)
    {
        EventInfo event = {};

        event.eventType = eEventType::Create_Object;
        event.objPointer = (DWORD_PTR)obj;
        event.objLayer = (DWORD_PTR)type;

        EventManager::AddEvent(event);
    }

    void DeleteObject(GameObject* obj, eLayerType type)
    {
        if (obj->IsDead())
            return;

        EventInfo event = {};

        event.eventType = eEventType::Delete_Object;
        event.objPointer = (DWORD_PTR)obj;
        event.objLayer = (DWORD_PTR)type;

        EventManager::AddEvent(event);
    }
}
