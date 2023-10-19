#include "zzSparkBolt.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzMesh.h"
#include "zzTexture.h"
#include "zzMaterial.h"
#include "zzResourceManager.h"
#include "zzTime.h"
#include "zzParticleSystem.h"
#include "zzCollider.h"
#include "zzPixelCollider_Lite.h"
#include "zzExplosionEffect.h"
#include "zzRigidBody.h"
#include "zzLight.h"
#include "zzAudioSource.h"
#include "zzObjectPoolManager.h"

namespace zz
{
    using namespace std;

    SparkBolt::SparkBolt()
        : mParticle(nullptr)
        , mSubParticle(nullptr)
        , mExplosion(nullptr)
        , mSubParticleTime(0.f)
        , mSleepTime(0.0f)
        , mbTimerOn(false)
        , mParticleData{}
        , mShareData{}
        , mIndex(0)
        , mTrailDuration(0.05f)
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"SparkBolt");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"SparkBolt_Idle", texture, Vector2(0.0f, 1.0f), Vector2(10.0f, 10.0f), 2, Vector2::Zero, 0.2f);
        ani->PlayAnimation(L"SparkBolt_Idle", true);

        GetComponent<Transform>()->SetScale(10.f, 10.f, 1.0f);
        AddComponent<Collider>()->SetScale(10.f, 4.f, 1.0f);

        mAudio = AddComponent<AudioSource>();
        mAudio->SetClip(ResourceManager::LoadAudioClip(L"SparkBolt_Sound", L"..\\Resources\\Audio\\Projectiles\\spell_shoot_ver1_1.wav"));
        mAudio->SetLoop(false);
        //mSound = ResourceManager::Find<AudioClip>(L"SparkBolt_Sound");
        //mSound = ResourceManager::Load<AudioClip>(L"crypt_s06_action_91", L"..\\Resources\\Audio\\Projectiles\\crypt_s06_action_91.wav");

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGravity(100.f);
        mRigid->SetRotate(true);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"SineFuncParticleCS"));

        mParticleData.distance.z = 0;
        mParticleData.randLifeTime = Vector2(0.001f, 0.5f);
        mParticleData.angle = GetComponent<Transform>()->GetRotation().z;
        mParticleData.scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mParticleData.color_min = Vector4(105.f / 255.f, 40.f / 255.f, 95.f / 255.f, 0.25f);
        mParticleData.color_max = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 0.5f);
        //mSharedData.randPositionMax = Vector2(1.0f, 0.0f);
        //mSharedData.randPositionMin = Vector2(-1.0f, 0.0f);
        mParticleData.randVelocityMax = Vector2(10.0f, 0.0f);
        //mParticleData.randVelocityMin = Vector2(0.0f, -10.0f);
        //mParticleData.lightScale = Vector4(3.0f, 3.0f, 1.0f, 1.0f);

        auto particles = std::make_shared<Particle[]>(250);
        mParticle->CreateStructedBuffer(sizeof(Particle), 250, eViewType::UAV, particles.get(), true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mSubParticle = AddComponent<ParticleSystem>();
        mSubParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mSubParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mSubParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCS"));

        auto subParticles = std::make_shared<Particle[]>(50);
        mSubParticle->CreateStructedBuffer(sizeof(Particle), 50, eViewType::UAV, subParticles.get(), true, 0, 14, 0);
        mSubParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShareData.color_min = mShareData.color_max = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 0.25f);

        mShareData.randPositionMax = Vector2(30.0f, 30.0f);
        mShareData.randPositionMin = Vector2(-30.0f, -30.0f);
        mShareData.randVelocityMax = Vector2(15.0f, -30.0f);
        mShareData.randVelocityMin = Vector2(-15.0f, 30.0f);
        mShareData.randLifeTime = Vector2(3.0f, 0.5f);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });
    }

    SparkBolt::~SparkBolt()
    {
    }

    void SparkBolt::Initialize()
    {
        mExplosion = new ExplosionEffect();
        std::shared_ptr<Texture> explosionTexture = ResourceManager::Find<Texture>(L"Explosion_SparkBolt");
        Animator* animator = new Animator();
        animator->Create(L"Explosion_SparkBolt_Play", explosionTexture, Vector2(0.0f, 1.0f), Vector2(9.0f, 9.0f), 8, Vector2::Zero, 0.01f);
        animator->PlayAnimation(L"Explosion_SparkBolt_Play", false);

        mExplosion->SetAnimator(animator, L"Explosion_SparkBolt_Play");

        mMuzzleEffect = new MuzzleEffect();
        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"Muzzle_SparkBolt");
        Animator* manimator = new Animator();
        manimator->Create(L"Muzzle_SparkBolt_Play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        manimator->PlayAnimation(L"Muzzle_SparkBolt_Play", false);

        mMuzzleEffect->SetAnimator(manimator, L"Muzzle_SparkBolt_Play");
        mMuzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        CreateObject(mMuzzleEffect, eLayerType::Effect);

        mSubParticleTime = 0.f;
        mSleepTime = 0.f;
        mbTimerOn = false;

        mIndex = 0;
        mTrailDuration = 0.05f;
        mSpeed = 500.f;
        mCastDelay = 0.05f;
        mDamage = 3.0f;
        mManaDrain = 5.0f;

        ProjectileSpell::Initialize();
    }

    void SparkBolt::Update()
    {
        if (mbTimerOn)
        {
            mSleepTime += (float)Time::DeltaTime();

            if (mSleepTime >= 3.f)
            {
                mParticle->OffParticle();
                mSubParticle->OffParticle();

                ObjectPoolManager::ReturnObjectToPool(this);
            }
            return;
        }


        if (mTime >= 0.8f && IsActive())
        {
            Dead();
        }

        ProjectileSpell::Update();
    }

    void SparkBolt::LateUpdate()
    {
        if (mbTimerOn)
        {
            mParticle->LateUpdate();
            mSubParticle->LateUpdate();

            return;
        }

        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        mParticleData.curPosition = curPos + 0.0f;
        mParticleData.distance = mParticleData.curPosition - mPrevPos;
        mParticleData.angle = GetComponent<Transform>()->GetRotation().z;

        UINT count = (UINT)max(fabs(mParticleData.distance.x), fabs(mParticleData.distance.y)) + 1;
        mParticleData.activeCount = count;
        mParticleData.totalActiveCount = count;
        mParticleData.index = mIndex;
        mIndex += count;
        
        mParticle->SetStructedBufferData(&mParticleData, 1, 1);

        mShareData.curPosition = curPos + 0.0f;
        mShareData.activeCount = random() > 0.9f ? 1 : 0;

        mSubParticle->SetStructedBufferData(&mShareData, 1, 1);

        ProjectileSpell::LateUpdate();
    }

    void SparkBolt::Render()
    {
        if (mbTimerOn)
        {
            mParticle->Render();
            mSubParticle->Render();

            return;
        }

        ProjectileSpell::Render();
    }

    void SparkBolt::Dead()
    {
        if (IsActive())
        {
            SetState(eState::Sleep);

            Vector3 pos = GetComponent<Transform>()->GetPosition() - (mDirection * 500.f * 0.005f);
            mExplosion->GetComponent<Transform>()->SetPosition(pos.x, pos.y, BACK_PIXEL_WORLD_Z);
            mExplosion->GetComponent<Transform>()->SetScale(9.0f, 9.0f, 1.0f);
            CreateObject(mExplosion, eLayerType::Effect);

            mbTimerOn = true;
        }
    }

    ProjectileSpell* SparkBolt::Clone()
    {
        return ObjectPoolManager::GetObjectInPool<SparkBolt>();
    }

    void SparkBolt::InitialSetting()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();
        mPrevPos = curPos + 0.0f;
        mShareData.curPosition = mPrevPos;
        mParticleData.curPosition = mPrevPos;

        mRigid->SetStartVelocity(mSpeed, mDirection);

        mAudio->Play();
    }

    void SparkBolt::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            GetComponent<PixelCollider_Lite>()->SetPositionPrevCollision();
            Dead();
        }
        else if (element.Type == eElementType::LIQUID)
        {
            mRigid->ApplyResistance(pow((0.0001f), (float)Time::DeltaTime()));
        }
    }
}