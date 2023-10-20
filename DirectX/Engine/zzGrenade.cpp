#include "zzGrenade.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzRigidBody.h"
#include "zzPixelCollider_Lite.h"
#include "zzLight.h"
#include "zzParticleSystem.h"
#include "zzCollider.h"
#include "zzAudioSource.h"

namespace zz
{
    Grenade::Grenade()
        : mParticle(nullptr)
        , mEffect(nullptr)
        , mRigid(nullptr)
        , mLight(nullptr)
        , mSpareTime(0.0f)
        , mStep(0)
        , mPrevPos(Vector3::Zero)
        , mShared{}
        , mCnt(0.0f)
    {
        mDamage = 17.f;
        mLimitTime = 8.0f;
        mSpeed = 120.f;
    }
    Grenade::~Grenade()
    {
        if (!IsDead())
        {
            delete mEffect;
        }
    }

    void Grenade::Initialize()
    {
        GetComponent<Transform>()->SetScale(12.f, 12.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"grenade_large");

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"grenade_large_idle", texture, Vector2(0.0f, 0.0f), Vector2(12.0f, 12.0f), 4, Vector2::Zero, 0.05f);
        animator->PlayAnimation(L"grenade_large_idle", true);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetStartVelocity(mSpeed, mAngle);
        mRigid->SetGravity(200.f);

        mCollider->SetScale(12.f, 12.f, 1.0f);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });
        
        mLight = AddComponent<Light>();
        mLight->SetLightType(5);
        mLight->SetLightScale(12.f, 12.f, 1.0f);
        mLight->SetLightColor(255.f / 255.f, 120.f / 255.f, 10.f / 255.f, 1.0f);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCS"));

        auto particles = std::make_shared<Particle[]>(450);
        mParticle->CreateStructedBuffer(sizeof(Particle), 450, eViewType::UAV, particles.get(), true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mShared.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShared.color_min = Vector4(255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.0f);
        mShared.color_max = Vector4(255.f / 255.f, 255.f / 255.f, 0.f / 255.f, 1.0f);
        mShared.randPositionMax = Vector2(5.0f, 5.0f);
        mShared.randPositionMin = Vector2(-5.0f, -5.0f);
        mShared.randVelocityMax = Vector2(15.0f, -2.0f);
        mShared.randVelocityMin = Vector2(-15.0f, -6.0f);
        mShared.randLifeTime = Vector2(0.3f, 0.08f);
        mShared.lightScale = Vector4(8.0f, 8.0f, 1.0f, 0.0f);

        MeshRenderer* particleLight = new MeshRenderer();
        particleLight->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        particleLight->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleLight(particleLight);

        mEffect = new ExplosionEffect();
        std::shared_ptr<Texture> eff_texture = ResourceManager::Find<Texture>(L"explosion_016");
        Animator* eff_animator = new Animator();
        eff_animator->Create(L"explosion_016_idle", eff_texture, Vector2(0.0f, 1.0f), Vector2(17.0f, 17.0f), 8, Vector2::Zero, 0.021f);
        eff_animator->PlayAnimation(L"explosion_016_idle", false);
        mEffect->SetAnimator(eff_animator, L"explosion_016_idle");
        mEffect->SetScale(17.f, 17.f, 1.0f);

        MonsterAttack::Initialize();
    }

    void Grenade::Update()
    {
        if (mStep == 1)
        {
            mSpareTime += (float)Time::DeltaTime();
        }
        mPrevPos = GetComponent<Transform>()->GetPosition();

        MonsterAttack::Update();
    }
    void Grenade::LateUpdate()
    {
        if (mStep > 0)
        {
            Vector3 curPos = GetComponent<Transform>()->GetPosition();
            mCnt +=  curPos.y - mPrevPos.y;

            if(mCnt >= 0.f)
            {
                int activeCnt = (int)mCnt;
                mCnt -= activeCnt;

                mShared.curPosition = curPos + 0.0f;
                mShared.activeCount = activeCnt * 8;
                mParticle->SetStructedBufferData(&mShared, 1, 1);
            }
        }

        MonsterAttack::LateUpdate();
    }
    void Grenade::Render()
    {
        MonsterAttack::Render();
    }

    void Grenade::OnCollisionEnter(GameObject* other)
    {
        MonsterAttack::OnCollisionEnter(other);
    }
    void Grenade::OnCollisionStay(GameObject* other)
    {
    }
    void Grenade::OnCollisionExit(GameObject* other)
    {
    }

    void Grenade::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            if (mStep == 0)
            {
                mStep++;

                mRigid->SetVelocityX(0.f);
                mRigid->SetVelocityY(250.f);
                mRigid->SetGravity(0.f);

                AudioSource* audio = AddComponent<AudioSource>();
                audio->SetClip(ResourceManager::LoadAudioClip(L"centipede_shoot_clean_01", L"..\\Resources\\Audio\\Enemy\\centipede_shoot_clean_01.wav"));
                audio->SetLoop(false);
                audio->SetVolume(50.f);
                audio->Play();
                
            }
            else
            {
                if(mSpareTime > 0.1f)
                {
                    mStep++;
                    dead();
                   
                }
            }
        }
    }

    void Grenade::dead()
    {
        if (!IsDead())
        {
            DeleteObject(this, eLayerType::MonsterAttack);

            Vector3 pos = GetComponent<Transform>()->GetPosition();
            mEffect->SetPosition(pos.x, pos.y, pos.z - 0.05f);
            CreateObject(mEffect, eLayerType::Effect);
        }
    }
}