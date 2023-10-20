#include "zzBomb.h"
#include "zzTransform.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzAnimator.h"
#include "zzCollider.h"
#include "zzBox2dCollider.h"
#include "zzExplosion_128.h"
#include "zzInput.h"
#include "zzAudioSource.h"
#include "zzObjectPoolManager.h"

namespace zz
{
    Bomb::Bomb()
    {
        mSpeed = 60.f;
        mCastDelay = 1.67f;
        mDamage = 3.0f;
        mManaDrain = 25.0f;

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"bomb");

        AddComponent<Box2dCollider>()->Create(Vector3(0.f, 0.f, 0.f), Vector3(8.0f, 8.0f, 1.0f));

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"Bomb_Idle", texture, Vector2(0.0f, 0.0f), Vector2(12.0f, 12.0f), 1, Vector2::Zero, 1.f);
        ani->PlayAnimation(L"Bomb_Idle", true);

        GetComponent<Transform>()->SetScale(12.f, 12.f, 1.0f);
        AddComponent<Collider>()->SetScale(10.f, 4.f, 1.0f);

        mAudio = AddComponent<AudioSource>();
        mAudio->SetClip(ResourceManager::LoadAudioClip(L"bomb_03", L"..\\Resources\\Audio\\Projectiles\\bomb_03.wav"));
        mAudio->SetLoop(false);
    }
    Bomb::~Bomb()
    {
    }

    void Bomb::Initialize()
    {
        mSpeed = 60.f;
        mCastDelay = 1.67f;
        mDamage = 3.0f;
        mManaDrain = 25.0f;

     
        ProjectileSpell::Initialize();
    }

    void Bomb::Update()
    {
        if (mTime >= 3.0f && IsActive())
        {
            Dead();
        }

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

    void Bomb::InitialSetting()
    {
        Vector3 mousePos = Input::GetMouseWorldPos();
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        float angle = std::atan2(mousePos.y - pos.y, mousePos.x - pos.x);

        Vector2 force = { 400.f, 0.f };
        force.x = 600.f * cos(angle);
        force.y = -600.f * sin(angle);

        GetComponent<Box2dCollider>()->Create(pos, Vector3(8.0f, 8.0f, 1.0f));
        GetComponent<Box2dCollider>()->ApplyLinearImpulse(force, Vector2(-1, -1));

        mMuzzleEffect = new MuzzleEffect();
        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"muzzle_launcher_large_01");
        Animator* manimator = new Animator();
        manimator->Create(L"muzzle_launcher_large_01_play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        manimator->PlayAnimation(L"muzzle_launcher_large_01_play", false);

        mMuzzleEffect->SetAnimator(manimator, L"muzzle_launcher_large_01_play");
        mMuzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        CreateObject(mMuzzleEffect, eLayerType::Effect);
    }

    Bomb* Bomb::Clone()
    {
        return ObjectPoolManager::GetObjectInPool<Bomb>();
    }

    void Bomb::Dead()
    {
        GetComponent<Box2dCollider>()->Release();

        ObjectPoolManager::ReturnObjectToPool(this);

        Vector3 pos = GetComponent<Transform>()->GetPosition();

        Explosion_128* bomb = new Explosion_128();
        bomb->GetComponent<Transform>()->SetPosition(pos);
        CreateObject(bomb, eLayerType::Effect);

        mAudio->Play();
    }
}