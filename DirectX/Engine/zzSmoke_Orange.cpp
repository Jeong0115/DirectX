#include "zzSmoke_Orange.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzMesh.h"
#include "zzAnimator.h"
#include "zzRigidBody.h"
#include "zzTransform.h"
#include "zzTime.h"
#include "zzObjectPoolManager.h"

namespace zz
{
    Smoke_Orange::Smoke_Orange()
        : mTime(0.f)
    {
        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"smoke_orange");

        MeshRenderer* meshRender = AddComponent<MeshRenderer>();
        meshRender->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        meshRender->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"smoke_orange_play", texture, Vector2(0.0f, 0.0f), Vector2(16.f, 16.f), 8, Vector2::Zero, 0.03f);
        animator->PlayAnimation(L"smoke_orange_play", false);

        RigidBody* rigid = AddComponent<RigidBody>();
        rigid->SetGravity(90.f);
        rigid->SetAirFirction(2.f);
        rigid->SetStartVelocity(randi(200) + 50.f, randf(6.283185f));

        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);
    }

    Smoke_Orange::~Smoke_Orange()
    {
    }

    void Smoke_Orange::Initialize()
    {
        GetComponent<RigidBody>()->SetStartVelocity(randi(200) + 50.f, randf(6.283185f));
        GetComponent<Animator>()->PlayAnimation(L"smoke_orange_play", false);

        mTime = 0.0f;

        GameObject::Initialize();
    }

    void Smoke_Orange::Update()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= 3.0f)
        {
            ObjectPoolManager::ReturnObjectToPool<Smoke_Orange>(this);
        }
        GameObject::Update();
    }

    void Smoke_Orange::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Smoke_Orange::Render()
    {
        GameObject::Render();
    }
}