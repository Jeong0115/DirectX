#include "zzInfoBoxTexture.h"
#include "zzResourceManager.h"
#include "zzMaterial.h"
#include "zzMeshRenderer.h"
#include "zzTransform.h"

namespace zz
{
    InfoBoxTexture::InfoBoxTexture(eUIType type)
        : UI(type)
    {
    }

    InfoBoxTexture::~InfoBoxTexture()
    {
    }

    void InfoBoxTexture::Initialize()
    {
        GameObject::Initialize();
    }

    void InfoBoxTexture::Update()
    {
        GameObject::Update();
    }

    void InfoBoxTexture::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void InfoBoxTexture::Render()
    {
        GameObject::Render();
    }

    void InfoBoxTexture::CreateItemTexture(std::wstring material_name, Vector3 scale)
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(material_name));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(scale);

        Initialize();
    }

    void InfoBoxTexture::OnCollisionEnter(GameObject* other)
    {
    }

    void InfoBoxTexture::OnCollisionStay(GameObject* other)
    {
    }

    void InfoBoxTexture::OnCollisionExit(GameObject* other)
    {
    }
}