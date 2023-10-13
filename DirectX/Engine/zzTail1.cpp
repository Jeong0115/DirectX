#include "zzTail1.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

#include "zzInput.h"
#include "zzTime.h"

namespace zz
{
    Tail1::Tail1()
    {
    }
    Tail1::~Tail1()
    {
    }
    void Tail1::Initialize()
    {
        GetComponent<Transform>()->SetScale(30.f, 31.f, 1.0f);
        GetComponent<Transform>()->SetPosition(0.f, -15.f, 0.23f);
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_tail"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GameObject::Initialize();
    }
    void Tail1::Update()
    {
        if (Input::GetKey(eKeyCode::R))
        {
            GetComponent<Transform>()->ShiftRotationZ(1.0f * (float)Time::DeltaTime());
        }
        if (Input::GetKey(eKeyCode::Q))
        {
            GetComponent<Transform>()->ShiftRotationZ(-1.0f * (float)Time::DeltaTime());
        }
        GameObject::Update();
    }
    void Tail1::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void Tail1::Render()
    {
        GameObject::Render();
    }
}