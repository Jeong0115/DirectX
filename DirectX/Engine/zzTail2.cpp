#include "zzTail2.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

namespace zz
{
    Tail2::Tail2()
    {
    }
    Tail2::~Tail2()
    {
    }
    void Tail2::Initialize()
    {
        GetComponent<Transform>()->SetScale(41.f, 42.f, 1.0f);
        GetComponent<Transform>()->SetPosition(0.f, -20.f, 0.23f);
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_tail_big"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GameObject::Initialize();
    }
    void Tail2::Update()
    {
        GameObject::Update();
    }
    void Tail2::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void Tail2::Render()
    {
        GameObject::Render();
    }
}