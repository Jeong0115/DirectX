#include "zzSpellSlot.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzCollider.h"
#include "zzWand.h"

namespace zz
{
    SpellSlot::SpellSlot(eUIType type)
        : UI(type)
        , mSlotIndex(9)
        , mSpell(nullptr)
        , mItemTexture(nullptr)
        , mbSlotInWand(false)
        , mOwnerWand(nullptr)
    {
    }

    SpellSlot::~SpellSlot()
    {
        if (mSpell != nullptr)
        {
            delete mSpell;
            mSpell = nullptr;
        }

        if (mItemTexture != nullptr)
        {
            delete mItemTexture;
            mItemTexture = nullptr;
        }
    }

    void SpellSlot::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(20.f, 20.f, 1.0f));

        Collider* collider = AddComponent<Collider>();
        collider->SetScale(Vector3(19.f, 19.f, 1.0f));

        GameObject::Initialize();
    }
    void SpellSlot::Update()
    {
        GameObject::Update();

        if (mItemTexture != nullptr)
        {
            mItemTexture->Update();
        }
    }

    void SpellSlot::LateUpdate()
    {
        GameObject::LateUpdate();

        if (mItemTexture != nullptr)
        {
            mItemTexture->LateUpdate();
        }
    }

    void SpellSlot::Render()
    {
        GameObject::Render();

        if (mItemTexture != nullptr)
        {
            mItemTexture->Render();
        }
    }

    void SpellSlot::OnCollisionEnter(GameObject* other)
    {
    }

    void SpellSlot::OnCollisionStay(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box_highlihgt"));       
        }
    }

    void SpellSlot::OnCollisionExit(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
        }
    }

    void SpellSlot::SetSpell(SpellUI* spell, ItemTexture* tex)
    {
        mSpell = spell;
        mItemTexture = tex;

        if (mItemTexture != nullptr)
        {
            mItemTexture->MoveSlot(GetComponent<Transform>()->GetWorldPosition());
            mItemTexture->SetSlotIndex(mSlotIndex);
            mItemTexture->SetOwner(this);
        }

        if (mbSlotInWand)
        {
            if (spell == nullptr)
            {
                mOwnerWand->SetSpell(nullptr, mSlotIndex);
            }
            else
            {
                mOwnerWand->SetSpell(spell->GetSpell(), mSlotIndex);
            }
        }
    }

    void SpellSlot::SetOwnerWand(Wand* wand)
    {
        mOwnerWand = wand;
        mbSlotInWand = true;
    }
}