#include "zzInfoBox.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzInventoryBox.h"
#include "zzInventoryManager.h"
#include "zzWand.h"

namespace zz
{
    InfoBox::InfoBox(eUIType type)
        : UI(type)
        , mWandtexture(nullptr)
        , mWand(nullptr)
    {
    }

    InfoBox::~InfoBox()
    {
        if (mWandtexture != nullptr)
        {
            delete mWandtexture;
            mWandtexture = nullptr;
        }
    }

    void InfoBox::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_info_box"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(150.f, 60.f, 1.0f));

        GameObject::Initialize();
    }

    void InfoBox::Update()
    {
        GameObject::Update();
        mWandtexture->Update();
    }

    void InfoBox::LateUpdate()
    {
        GameObject::LateUpdate();
        mWandtexture->LateUpdate();
    }

    void InfoBox::Render()
    {
        GameObject::Render();
        mWandtexture->Render();
    }

    void InfoBox::LinkWand(Wand* wand, InfoBoxTexture* texture)
    {
        Transform* tr = GetComponent<Transform>();

        mWand = wand;
        mWandtexture = texture;

        mWandtexture->GetComponent<Transform>()->SetParent(tr);
        mWandtexture->GetComponent<Transform>()->SetPosition(-55.f, 10.f, 0.f);

        for (int i = 0; i < mWand->GetCapacity(); i++)
        {
            InventoryBox* spellBox = new InventoryBox(eUIType::WandSlot);
            Transform* boxTr = spellBox->GetComponent<Transform>();
            boxTr->SetParent(tr);
            boxTr->SetPosition(-60.f + i * 20.f, -15.f, 0.0f);
            spellBox->Initialize();

            InventoryManager::AddUIObject(spellBox, eUIType::WandSlot);
        }
    }

    void InfoBox::OnCollisionEnter(GameObject* other)
    {
    }

    void InfoBox::OnCollisionStay(GameObject* other)
    {
    }

    void InfoBox::OnCollisionExit(GameObject* other)
    {
    }
}