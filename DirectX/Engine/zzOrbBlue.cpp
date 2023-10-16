#include "zzOrbBlue.h"
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
    OrbBlue::OrbBlue(int type)
        : mEffect(nullptr)
        , mType(type)
        , mOrbitalCenter(Vector3::Zero)
        , mOrbitalTime(0.0f)
        , mRigid(nullptr)
    {
        mDamage = 15.f;
        mLimitTime = 3.0f;
        mSpeed = 120.f;
    }

    OrbBlue::~OrbBlue()
    {
    }

    void OrbBlue::Initialize()
    {
        if (mType != 2)
        {
            GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);
            mCollider->SetScale(12.f, 12.f, 1.0f);
        }


        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"orb_blue_big");

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"orb_blue_big_idle", texture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 4, Vector2::Zero, 0.05f);
        animator->PlayAnimation(L"orb_blue_big_idle", true);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGravity(0.0f);

        if (mType == 1)
        {
            mRigid->SetStartVelocity(mSpeed, mDirection);
            mOrbitalCenter = GetComponent<Transform>()->GetPosition();
            mLimitTime = 1.0f;
        }
        else
        {
            mRigid->SetStartVelocity(mSpeed, mAngle);
            mOrbitalCenter = GetComponent<Transform>()->GetPosition();
        }

        //Light* light = AddComponent<Light>();
        //light->SetLightType(0);
        //light->SetLightScale(16.f, 17.f, 1.0f);
        //light->SetLightColor(126.f / 255.f, 52.f / 255.f, 255.f / 255.f, 0.6f);

        mEffect = new ExplosionEffect();
        std::shared_ptr<Texture> eff_texture = ResourceManager::Find<Texture>(L"explosion_016_plasma");
        Animator* eff_animator = new Animator();
        eff_animator->Create(L"explosion_016_plasma_idle", eff_texture, Vector2(0.0f, 0.0f), Vector2(17.0f, 17.0f), 8, Vector2::Zero, 0.021f);
        eff_animator->PlayAnimation(L"explosion_016_plasma_idle", false);
        mEffect->SetAnimator(eff_animator, L"explosion_016_plasma_idle");
        mEffect->SetScale(17.f, 17.f, 1.0f);

        MonsterAttack::Initialize();
    }

    void OrbBlue::Update()
    {
        mOrbitalTime += (float)Time::DeltaTime();
        if (mTime >= 1.0f)
        {

        }
        if (mType == 0 && mOrbitalTime >= 1.0f)
        {
            mRigid->SetOrbitalMotion(true, mOrbitalCenter);
        }
        MonsterAttack::Update();
    }

    void OrbBlue::LateUpdate()
    {
        MonsterAttack::LateUpdate();
    }
    void OrbBlue::Render()
    {
        MonsterAttack::Render();
    }

    void OrbBlue::OnCollisionEnter(GameObject* other)
    {
        MonsterAttack::OnCollisionEnter(other);
    }

    void OrbBlue::OnCollisionStay(GameObject* other)
    {
    }
    void OrbBlue::OnCollisionExit(GameObject* other)
    {
    }

    void OrbBlue::dead()
    {
        if(!IsDead())
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();
            mEffect->SetPosition(pos.x, pos.y, pos.z - 0.05f);
            CreateObject(mEffect, eLayerType::Effect);

            if (mType == 1)
            {
                for (int i = 0; i < 4; i++)
                {
                    OrbBlue* blue = new OrbBlue(2);
                    blue->SetAngle(PI * 2. * i / 4.);
                    blue->SetSpeed(mSpeed * 1.7f);
                    blue->SetPosition(pos);
                    if (i % 2 == 0)
                    {
                        blue->GetComponent<Transform>()->SetScale(24.f, 8.f, 1.0f);
                        blue->GetComponent<Collider>()->SetScale(20.f, 6.f, 1.0f);
                    }
                    else
                    {
                        blue->GetComponent<Transform>()->SetScale(8.f, 24.f, 1.0f);
                        blue->GetComponent<Collider>()->SetScale(6.f, 20.f, 1.0f);
                    }
                    CreateObject(blue, eLayerType::MonsterAttack);
                }
            }
            DeleteObject(this, eLayerType::MonsterAttack);
        }
    }
}