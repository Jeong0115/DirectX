#include "zzGameObject.h"
#include "zzRenderer.h"
#include "zzGraphicsDevice.h"
#include "zzTransform.h"


namespace zz
{
    GameObject::GameObject()
        : mState(eState::Active)
        , mbActive(true)
        , mLayerType(eLayerType::End)
    {
        AddComponent<Transform>();
    }

    GameObject::~GameObject()
    {
        for (Component* comp : mComponents)
        {
            if (comp == nullptr)
                continue;

            delete comp;
            comp = nullptr;
        }
    }

    void GameObject::Initialize()
    {
        for (Component* comp : mComponents)
        {
            comp->Initialize();
        }
    }

    void GameObject::Update()
    {
        for (Component* comp : mComponents)
        {
            comp->Update();
        }
    }

    void GameObject::LateUpdate()
    {
        for (Component* comp : mComponents)
        {
            comp->LateUpdate();
        }
    }

    void GameObject::Render()
    {
        for (Component* comp : mComponents)
        {
            comp->Render();
        }
    }
    void GameObject::OnCollisionEnter(GameObject* other)
    {
    }
    void GameObject::OnCollisionStay(GameObject* other)
    {
    }
    void GameObject::OnCollisionExit(GameObject* other)
    {
    }
}