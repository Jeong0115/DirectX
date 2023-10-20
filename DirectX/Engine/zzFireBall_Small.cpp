#include "zzFireBall_Small.h"
#include "zzHealthPoint.h"
#include "zzRigidBody.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzAnimator.h"
#include "zzPixelCollider_Lite.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzCollider.h"
#include "zzAudioSource.h"

namespace zz
{
    FireBall_Small::FireBall_Small()
        : mRigid(nullptr)
    {
        mDamage = 12.f;
        mLimitTime = 2.0f;
        mSpeed = 400.f;
        mDirection = (Vector3::Zero);
    }
    FireBall_Small::~FireBall_Small()
    {
    }

    void FireBall_Small::Initialize()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"fireball_small", L"..\\Resources\\Texture\\Monster\\fireball_small.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();

        animator->Create(L"fireball_small_play", texture, Vector2(0.0f, 0.0f), Vector2(4.0f, 4.0f), 4, Vector2::Zero, 0.02f);
        animator->PlayAnimation(L"fireball_small_play", true);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGround(true);
        mRigid->SetStartVelocity(mSpeed, mDirection);

        mCollider->SetScale(4.0f, 4.0f, 1.0f);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });

        AudioSource* audio = AddComponent<AudioSource>();
        audio->SetClip(ResourceManager::LoadAudioClip(L"shotgun_02", L"..\\Resources\\Audio\\Projectiles\\shotgun_02.wav"));
        //audio->SetClip(ResourceManager::Load<AudioClip>(L"shotgun_02", L"..\\Resources\\Audio\\Projectiles\\shotgun_02.wav"));
        audio->SetLoop(false);
        audio->Play();
        MonsterAttack::Initialize();
    }

    void FireBall_Small::Update()
    {
        MonsterAttack::Update();
    }

    void FireBall_Small::LateUpdate()
    {
        MonsterAttack::LateUpdate();
    }

    void FireBall_Small::Render()
    {
        MonsterAttack::Render();
    }

    void FireBall_Small::SetDetectPos(Vector3 pos)
    {
        Vector3 myPos = GetComponent<Transform>()->GetPosition();
        pos -= myPos;

        pos.Normalize(mDirection);
    }

    void FireBall_Small::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            dead();
        }
        else if (element.Type == eElementType::LIQUID)
        {
            mRigid->ApplyResistance(pow((0.01f), (float)Time::DeltaTime()));
        }
    }
    void FireBall_Small::dead()
    {
        if(!IsDead())
        {
            DeleteObject(this, eLayerType::MonsterAttack);
        }
    }
}