#include "zzBarBackGround.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"

namespace zz
{
    BarBackGround::BarBackGround()
        : UI(eUIType::HUD)
    {
    }

    BarBackGround::~BarBackGround()
    {
    }

    void BarBackGround::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_colors_bar_bg"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GameObject::Initialize();
    }

    void BarBackGround::Update()
    {
        GameObject::Update();
    }

    void BarBackGround::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void BarBackGround::Render()
    {
        GameObject::Render();
    }

    void BarBackGround::OnCollisionEnter(GameObject* other)
    {
    }

    void BarBackGround::OnCollisionStay(GameObject* other)
    {
    }

    void BarBackGround::OnCollisionExit(GameObject* other)
    {
    }
}