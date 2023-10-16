#include "function.h"
#include "zzEventManager.h"
#include "zzGameObject.h"
#include "zzTransform.h"


namespace zz
{
    void CreateObject(GameObject* obj, eLayerType type, float x, float y, float z)
    {
        EventInfo event = {};

        if(x != 0.0f)
        {
            obj->GetComponent<Transform>()->SetPosition(x, y, z);
        }

        event.eventType = eEventType::Create_Object;
        event.objPointer = (DWORD_PTR)obj;
        event.objLayer = (DWORD_PTR)type;

        EventManager::AddEvent(event);
    }

    void DeleteObject(GameObject* obj, eLayerType type)
    {
        if (obj->IsDead())
            return;
        
        obj->SetLastUpdate();

        EventInfo event = {};

        event.eventType = eEventType::Delete_Object;
        event.objPointer = (DWORD_PTR)obj;
        event.objLayer = (DWORD_PTR)type;

        EventManager::AddEvent(event);
    }
}
