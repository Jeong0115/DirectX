#include "zzTail1.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

#include "zzInput.h"
#include "zzTime.h"

namespace zz
{
    Tail1::Tail1()
        : mAngle(0.f)
        , mDir(1)
        , mbDead(false)
    {
    }
    Tail1::~Tail1()
    {
    }
    void Tail1::Initialize()
    {
        mDir = random() > 0.5f ? 1 : -1;
        GetComponent<Transform>()->SetScale(30.f, 31.f, 1.0f);
        GetComponent<Transform>()->SetPosition(0.f, -15.f, 0.23f);
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_tail"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GameObject::Initialize();
    }
    void Tail1::Update()
    {
        if(!mbDead)
        {
            mAngle += randf(2.0f) * mDir * (float)Time::DeltaTime();

            if (mAngle >= 0.5f * PI || mAngle <= -0.5f * PI)
            {
                mDir *= -1;
            }
            GetComponent<Transform>()->SetRotationZ(mAngle);
        }
       //int dir = random() > 0.5f ? 1 : -1;
       //GetComponent<Transform>()->ShiftRotationZ(dir* random() * 0.5f* (float)Time::DeltaTime());
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