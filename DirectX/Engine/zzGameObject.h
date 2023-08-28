#pragma once

#include "zzEntity.h"
#include "zzComponent.h"

namespace zz
{
    template<class T>
    concept DerivedFromComponent = std::derived_from<T, Component>;

    class GameObject : public Entity
    {
    public:
        enum eState
        {
            Active,
            Paused,
            Dead,
        };

        GameObject();
        virtual ~GameObject();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void OnCollisionEnter(GameObject* other);
        virtual void OnCollisionStay(GameObject* other);
        virtual void OnCollisionExit(GameObject* other);
 
        template<DerivedFromComponent T>
        T* GetComponent(/*const std::wstring name = "no_select"*/)
        {
            for (Component* comp : mComponents)
            {
                if (dynamic_cast<T*>(comp))
                    return dynamic_cast<T*>(comp);
            }
            return nullptr;
        }

        template<DerivedFromComponent T>
        T* AddComponent()
        {
            T* comp = new T();
            comp->SetOwner(this);
            mComponents.push_back(comp);
            
            return comp;
        }

        template<DerivedFromComponent T>
        void AddComponent(T* component)
        {
            component->SetOwner(this);
            mComponents.push_back(component);
        }

        eState GetState() { return mState; }
        bool IsDead() { return (mState == eState::Dead); }
        bool GetActive() { return mbActive; }
        void SetActive(bool active) { mbActive = active; }

    protected:
        void SetState(eState state) { mState = state; }
        friend class EventManager;

    private:
        eState  mState;
        bool    mbActive;
        std::vector<Component*> mComponents;
    };
}
