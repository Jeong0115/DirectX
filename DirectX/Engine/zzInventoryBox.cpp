#include "zzInventoryBox.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzCollider.h"

namespace zz
{
    InventoryBox::InventoryBox(eUIType type)
        : UI(type)
        , mSlotIndex(9)
        , mItem(nullptr)
        , mItemTexture(nullptr)
    {
    }

    InventoryBox::~InventoryBox()
    {
        if (mItem != nullptr)
        {
            delete mItem;
            mItem = nullptr;
        }

        if (mItemTexture != nullptr)
        {
            delete mItemTexture;
            mItemTexture = nullptr;
        }
    }

    void InventoryBox::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(20.f, 20.f, 1.0f));

        Collider* collider = AddComponent<Collider>();
        collider->SetScale(Vector3(19.f, 19.f, 1.0f));

        GameObject::Initialize();
    }
    void InventoryBox::Update()
    {
        GameObject::Update();

        if (mItemTexture != nullptr)
        {
            mItemTexture->Update();
        }
    }

    void InventoryBox::LateUpdate()
    {
        GameObject::LateUpdate();

        if (mItemTexture != nullptr)
        {
            mItemTexture->LateUpdate();
        }
    }

    void InventoryBox::Render()
    {
        GameObject::Render();

        if (mItemTexture != nullptr)
        {
            mItemTexture->Render();
        }
    }

    void InventoryBox::OnCollisionEnter(GameObject* other)
    {
    }

    void InventoryBox::OnCollisionStay(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box_highlihgt"));
        }
    }

    void InventoryBox::OnCollisionExit(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
        }
    }

    void InventoryBox::SetItem(Equipment* item, ItemTexture* tex)
    {
        mItem = item; 
        mItemTexture = tex;
        
        if(mItemTexture != nullptr)
        {
            mItemTexture->MoveSlot(GetComponent<Transform>()->GetPosition());
            mItemTexture->SetSlotIndex(mSlotIndex);
        }
    }
}