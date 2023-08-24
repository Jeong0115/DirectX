#include "zzShotGunner_Weak.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzAnimation.h"

namespace zz
{
    ShotGunner_Weak::ShotGunner_Weak()
        : mAnimator(nullptr)
    {
    }

    ShotGunner_Weak::~ShotGunner_Weak()
    {
    }

    void ShotGunner_Weak::Initialize()
    {
        GetComponent<Transform>()->SetScale(17.f, 17.f, 1.0f);

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"shotgunner_weak");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        mAnimator = AddComponent<Animator>();

        mAnimator->Create(L"shotgunner_weak_stand", texture, Vector2(0.0f, 0.0f), Vector2(17.0f, 17.0f), 6, Vector2::Zero, 0.16f);
        mAnimator->Create(L"shotgunner_weak_walk", texture, Vector2(0.0f, 17.f), Vector2(17.0f, 17.0f), 6, Vector2::Zero, 0.11f);
        mAnimator->Create(L"shotgunner_weak_attack", texture, Vector2(0.0f, 68.f), Vector2(17.0f, 17.0f), 6, Vector2::Zero, 0.07f);
        mAnimator->Create(L"shotgunner_weak_attack_ranged", texture, Vector2(0.0f, 85.f), Vector2(17.0f, 17.0f), 7, Vector2::Zero, 0.15f);

        mAnimator->FindAnimation(L"shotgunner_weak_stand")->SetAnimationEvent(3, std::bind(&ShotGunner_Weak::Test, this));
        mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
        GameObject::Initialize();
    }

    void ShotGunner_Weak::Update()
    {
        GameObject::Update();
    }

    void ShotGunner_Weak::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void ShotGunner_Weak::Render()
    {
        GameObject::Render();
    }

    void ShotGunner_Weak::OnCollisionEnter(GameObject* other)
    {
    }

    void ShotGunner_Weak::OnCollisionStay(GameObject* other)
    {
    }

    void ShotGunner_Weak::OnCollisionExit(GameObject* other)
    {
    }

    void ShotGunner_Weak::Test()
    {
        int a = 0;
    }
}