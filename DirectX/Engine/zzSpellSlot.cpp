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
        , mbMouseOn(false)
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

            if (mbMouseOn)
            {
                mTextBox->Render();
            }
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
            mbMouseOn = true;
        }
    }

    void SpellSlot::OnCollisionExit(GameObject* other)
    {
        if (dynamic_cast<UI*>(other)->GetUIType() == eUIType::Mouse)
        {
            MeshRenderer* mesh = GetComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_inventory_box"));
            mbMouseOn = false;
        }
    }

    void SpellSlot::SetSpell(SpellUI* spell, ItemTexture* tex)
    {
        mSpell = spell;
        mItemTexture = tex;

        if (mItemTexture != nullptr)
        {
            Vector3 pos = GetComponent<Transform>()->GetWorldPosition();

            mItemTexture->MoveSlot(pos);
            mItemTexture->SetSlotIndex(mSlotIndex);
            mItemTexture->SetOwner(this);

            mTextBox = spell->GetTextBox();
            Transform* texTr = mTextBox->GetComponent<Transform>();
            Vector3 scale = texTr->GetScale();

            if (pos.x - scale.x / 2 - 15.f <= 0.f)
            {
                pos.x -= pos.x - scale.x / 2 - 15.f;
            }

            texTr->SetPosition(pos.x, pos.y - scale.y / 2 - 15.f, 0.0f);

        }
        else
        {
            mTextBox = nullptr;
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