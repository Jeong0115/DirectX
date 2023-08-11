#include "zzParticle.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzMesh.h"
#include "zzTransform.h"
#include "zzResourceManager.h"

namespace zz
{
    TempParticle::TempParticle()
        : mVelocity(1.f, 1.f, 0.f)
        , mLifeTime(0.15f)
    {
    }

    TempParticle::~TempParticle()
    {
    }

    void TempParticle::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_Particle_Pink"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(1.f, 1.f, 1.f);

        GameObject::Initialize();
    }

    void TempParticle::Update()
    {
        mPassedTime += (float)Time::DeltaTime();

        if (mPassedTime <= mLifeTime)
        {
            Transform* tr = GetComponent<Transform>();
            Vector3 pos = tr->GetPosition();

            pos += mVelocity;

            tr->SetPosition(pos);           
        }
        else
        {
            DeleteObject(this, eLayerType::Particle);
        }

        GameObject::Update();
    }

    void TempParticle::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void TempParticle::Render()
    {
        GameObject::Render();
    }
}