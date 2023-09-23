#include "zzItemTexture.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzMaterial.h"
#include "zzMeshRenderer.h"
#include "zzTransform.h"
#include "zzInput.h"
#include "zzCollider.h"
#include "zzTime.h"

namespace zz
{
    ItemTexture::ItemTexture(eUIType type)
        : UI(type)
        , mTextureType(eTextureType::Item)
        , mSlotPos(Vector3::Zero)
        , mSlotIndex(9)
        , mbMove(false)
        , mOwner(nullptr)
    {
    }

    ItemTexture::~ItemTexture()
    {
    }

    void ItemTexture::Initialize()
    {
        AddComponent<Collider>();
        GameObject::Initialize();
    }

    void ItemTexture::Update()
    {
        if (mbMove)
        {
            Transform* transform = GetComponent<Transform>();
            Vector3 pos = transform->GetPosition();

            if ((pos.x - mSlotPos.x) * (pos.x - mSlotPos.x) + (pos.y - mSlotPos.y) * (pos.y - mSlotPos.y) <= 15.0f)
            {
                transform->SetPosition(mSlotPos);
                mbMove = false;
            }
            else
            {
                Vector3 direction = mSlotPos - pos;
                direction.Normalize();

                transform->SetPosition(pos + direction * 400.f * (float)Time::DeltaTime());
            }
        }

        GameObject::Update();
    }

    void ItemTexture::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void ItemTexture::Render()
    {
        GameObject::Render();
    }

    void ItemTexture::CreateItemTexture(std::wstring material_name, eTextureType type, Vector3 pos, Vector3 scale)
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(material_name));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(scale);
        GetComponent<Transform>()->SetPosition(pos);

        mSlotPos = pos;
        mTextureType = type;

        Initialize();
    }

    void ItemTexture::MoveSlot(Vector3 pos)
    {
        mSlotPos = pos;
        mbMove = true;
    }

    void ItemTexture::MoveOriginSlot()
    {
        mbMove = true;
    }


    void ItemTexture::OnCollisionEnter(GameObject* other)
    {
        
    }

    void ItemTexture::OnCollisionStay(GameObject* other)
    {
        
    }

    void ItemTexture::OnCollisionExit(GameObject* other)
    {
    }
}