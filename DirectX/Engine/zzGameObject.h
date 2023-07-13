#pragma once

#include "zzEntity.h"
#include "zzComponent.h"
//#include <type_traits>
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

        //template <class T, class = class std::enable_if<std::is_base_of<Component, T>::value>::type>   
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

    private:
        eState mState;
        std::vector<Component*> mComponents;
    };
}
