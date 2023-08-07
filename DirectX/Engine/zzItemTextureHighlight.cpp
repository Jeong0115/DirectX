#include "zzItemTextureHighlight.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzResourceManager.h"

namespace zz
{
    ItemTextureHighlight::ItemTextureHighlight(eUIType type)
        : UI(type)
    {
    }

    ItemTextureHighlight::~ItemTextureHighlight()
    {
    }

    void ItemTextureHighlight::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_highlight"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
        GetComponent<Transform>()->SetScale(19.f, 19.f, 1.f);

        GameObject::Initialize();
    }

    void ItemTextureHighlight::Update()
    {
        GameObject::Update();
    }

    void ItemTextureHighlight::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void ItemTextureHighlight::Render()
    {
        GameObject::Render();
    }

    void ItemTextureHighlight::OnCollisionEnter(GameObject* other)
    {
    }

    void ItemTextureHighlight::OnCollisionStay(GameObject* other)
    {
    }

    void ItemTextureHighlight::OnCollisionExit(GameObject* other)
    {
    }
}