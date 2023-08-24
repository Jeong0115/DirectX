#include "zzMouse.h"
#include "zzCollider.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzInput.h"
#include "zzMaterial.h"
#include "zzItemTexture.h"
#include "zzInventoryBox.h"
#include "zzInventoryManager.h"

namespace zz
{
    Mouse::Mouse(eUIType type)
        : UI(type)
        , mControllUI(nullptr)
    {
    }
    Mouse::~Mouse()
    {
    }
    void Mouse::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_mouse_cursor"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Vector3 pos = Input::GetMouseUIPos();
        GetComponent<Transform>()->SetPosition(pos.x, pos.y, 0.0f);
        GetComponent<Transform>()->SetScale(13.f, 13.f, 1.f);

        Collider* collider = AddComponent<Collider>();
        collider->SetScale(Vector3(1.f, 1.f, 1.f));

        GameObject::Initialize();
    }

    void Mouse::Update()
    {
        Vector3 pos = Input::GetMouseUIPos();
        GetComponent<Transform>()->SetPosition(pos.x, pos.y, 0.0f);

        if (mControllUI != nullptr)
        {
            if (Input::GetKeyUp(eKeyCode::LBUTTON))
            {
                mControllUI->MoveOriginSlot();
                mControllUI = nullptr;
            }
            else
            {
                mControllUI->GetComponent<Transform>()->SetPosition(pos.x + 10.f, pos.y - 10.f, 0.0f);
            }
        }

        GameObject::Update();
    }

    void Mouse::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void Mouse::Render()
    {
        GameObject::Render();
    }

    void Mouse::OnCollisionEnter(GameObject* other)
    {
    }

    void Mouse::OnCollisionStay(GameObject* other)
    {
        UI* uiObject = dynamic_cast<UI*>(other);

        if(uiObject != nullptr)
        {
            if (uiObject->GetUIType() == eUIType::ItemSlot && Input::GetKeyDown(eKeyCode::LBUTTON))
            {       
                mControllUI = dynamic_cast<InventoryBox*>(uiObject)->GetItemTexture();
            }
            else if (uiObject->GetUIType() == eUIType::ItemSlot && Input::GetKeyUp(eKeyCode::LBUTTON))
            {
                if (mControllUI != nullptr)
                {
                    UINT prevSlotIndex = mControllUI->GetSlotIndex();
                    UINT moveSlotIndex = dynamic_cast<InventoryBox*>(uiObject)->GetSlotIndex();

                    InventoryManager::MoveItemToSlot(prevSlotIndex, moveSlotIndex);

                    mControllUI = nullptr;
                }
            }
        }
    }

    void Mouse::OnCollisionExit(GameObject* other)
    {
    }
}