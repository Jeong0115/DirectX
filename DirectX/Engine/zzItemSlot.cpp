#include "zzItemSlot.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzCollider.h"

namespace zz
{
    ItemSlot::ItemSlot(eUIType type)
        : UI(type)
        , mSlotIndex(9)
        , mItem(nullptr)
        , mItemTexture(nullptr)
        , mTextBox(nullptr)
        , mbMouseOn(false)
    {
    }

    ItemSlot::~ItemSlot()
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

        delete mTextBox;
    }

    void ItemSlot::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(20.f, 20.f, 1.0f));

        Collider* collider = AddComponent<Collider>();
        collider->SetScale(Vector3(19.f, 19.f, 1.0f));

        GameObject::Initialize();
    }
    void ItemSlot::Update()
    {
        GameObject::Update();

        if (mItemTexture != nullptr)
        {
            mItemTexture->Update();
        }
    }

    void ItemSlot::LateUpdate()
    {
        GameObject::LateUpdate();

        if (mItemTexture != nullptr)
        {
            mItemTexture->LateUpdate();
        }
    }

    void ItemSlot::Render()
    {
        GameObject::Render();

        if (mItemTexture != nullptr)
        {
            mItemTexture->Render();

            if (mbMouseOn)
            {
                mTextBox->Render();
            }
        }

        
    }

    void ItemSlot::OnCollisionEnter(GameObject* other)
    {
    }

    void ItemSlot::OnCollisionStay(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box_highlihgt"));
            mbMouseOn = true;
        }
    }

    void ItemSlot::OnCollisionExit(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
            mbMouseOn = false;
        }
    }

    void ItemSlot::SetItem(Equipment* item, ItemTexture* tex)
    {
        mItem = item; 
        mItemTexture = tex;
        
        if(mItemTexture != nullptr)
        {
            mItemTexture->MoveSlot(GetComponent<Transform>()->GetPosition());
            mItemTexture->SetSlotIndex(mSlotIndex);

            mTextBox = mItem->GetTextBox();
            mTextBox->GetComponent<Transform>()->SetPosition(Vector3(85.f, 220.f, 0.f));
        }
        else
        {
            mTextBox = nullptr;
        }
    }
}