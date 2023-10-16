#include "zzOrbPink.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzRigidBody.h"
#include "zzLight.h"
#include "zzCollider.h"

namespace zz
{
    OrbPink::OrbPink()
        : mEffect(nullptr)
    {
        mDamage = 15.f;
        mLimitTime = 2.0f;
        mSpeed = 80.f;
    }
    OrbPink::~OrbPink()
    {
    }

    void OrbPink::Initialize()
    {
        GetComponent<Transform>()->SetScale(17.f, 17.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"orb_pink_glowy");

        mCollider->SetScale(13.f, 13.f, 1.0f);

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"orb_pink_glowy_idle", texture, Vector2(0.0f, 0.0f), Vector2(17.0f, 17.0f), 4, Vector2::Zero, 0.06f);
        animator->PlayAnimation(L"orb_pink_glowy_idle", true);

        RigidBody* rigid = AddComponent<RigidBody>();
        rigid->SetStartVelocity(mSpeed, mAngle);
        rigid->SetGravity(0.0f);

        Light* light = AddComponent<Light>();
        light->SetLightType(0);
        light->SetLightScale(17.f, 17.f, 1.0f);
        light->SetLightColor(216.f / 255.f, 52.f / 255.f, 163.f / 255.f, 0.6f);

        mEffect = new ExplosionEffect();
        std::shared_ptr<Texture> eff_texture = ResourceManager::Find<Texture>(L"explosion_016_plasma");
        Animator* eff_animator = new Animator();
        eff_animator->Create(L"explosion_016_plasma_idle", eff_texture, Vector2(0.0f, 0.0f), Vector2(17.0f, 17.0f), 8, Vector2::Zero, 0.021f);
        eff_animator->PlayAnimation(L"explosion_016_plasma_idle", false);
        mEffect->SetAnimator(eff_animator, L"explosion_016_plasma_idle");
        mEffect->SetScale(17.f, 17.f, 1.0f);

        MonsterAttack::Initialize();
    }

    void OrbPink::Update()
    {
        MonsterAttack::Update();
    }

    void OrbPink::LateUpdate()
    {
        MonsterAttack::LateUpdate();
    }

    void OrbPink::Render()
    {
        MonsterAttack::Render();
    }

    void OrbPink::OnCollisionEnter(GameObject* other)
    {
        MonsterAttack::OnCollisionEnter(other);
    }
    void OrbPink::OnCollisionStay(GameObject* other)
    {
    }
    void OrbPink::OnCollisionExit(GameObject* other)
    {
    }

    void OrbPink::dead()
    {
        if(!IsDead())
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();
            mEffect->SetPosition(pos.x, pos.y, pos.z - 0.05f);
            CreateObject(mEffect, eLayerType::Effect);

            DeleteObject(this, eLayerType::MonsterAttack);
        }
    }
}