#include "zzBomb.h"
#include "zzTransform.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzAnimator.h"
#include "zzCollider.h"
#include "zzBox2dCollider.h"

namespace zz
{
    Bomb::Bomb()
    {
        mSpeed = 60.f;
        mCastDelay = 1.67f;
        mDamage = 3.0f;
        mManaDrain = 25.0f;
    }
    Bomb::~Bomb()
    {
    }

    void Bomb::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"bomb");

        AddComponent<Box2dCollider>();

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"Bomb_Idle", texture, Vector2(0.0f, 0.0f), Vector2(12.0f, 12.0f), 1, Vector2::Zero, 1.f);
        ani->PlayAnimation(L"Bomb_Idle", true);

        GetComponent<Transform>()->SetScale(12.f, 12.f, 1.0f);
        AddComponent<Collider>()->SetScale(10.f, 4.f, 1.0f);

        ProjectileSpell::Initialize();

        GetComponent<Box2dCollider>()->ApplyLinearImpulse(Vector2(500.f, 30.f), Vector2(-1,-1));

        mMuzzleEffect = new MuzzleEffect();
        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"muzzle_launcher_large_01");
        Animator* manimator = new Animator();
        manimator->Create(L"muzzle_launcher_large_01_play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        manimator->PlayAnimation(L"muzzle_launcher_large_01_play", false);

        mMuzzleEffect->SetAnimator(manimator, L"muzzle_launcher_large_01_play");
        mMuzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        CreateObject(mMuzzleEffect, eLayerType::Effect);
    }

    void Bomb::Update()
    {
        ProjectileSpell::Update();
    }

    void Bomb::LateUpdate()
    {
        ProjectileSpell::LateUpdate();
    }

    void Bomb::Render()
    {
        ProjectileSpell::Render();
    }

    Bomb* Bomb::Clone()
    {
        return new Bomb();
    }
}