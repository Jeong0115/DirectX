#include "zzMagicArrow.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzTime.h"
#include "zzParticleSystem.h"
#include "zzCollider.h"
#include "zzPixelCollider_Lite.h"
#include "zzExplosionEffect.h"
#include "zzRigidBody.h"
#include "zzLight.h"
#include "zzAudioSource.h"

#include "zzPixelWorld.h"
#include "zzElement.h"
#include "zzObjectPoolManager.h"

namespace zz
{
    using namespace std;

    MagicArrow::MagicArrow()
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
        mSpeed = 350.f;
        mCastDelay = 0.05f;
        mDamage = 10.0f;
        mManaDrain = 20.0f;

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"light_arrow");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"light_arrow_Idle", texture, Vector2(0.0f, 0.0f), Vector2(9.0f, 5.0f), 1, Vector2::Zero, 1.0f);
        ani->PlayAnimation(L"light_arrow_Idle", true);

        GetComponent<Transform>()->SetScale(9.f, 5.f, 1.0f);
        AddComponent<Collider>()->SetScale(9.f, 5.f, 1.0f);

        Light* light = AddComponent<Light>();
        light->SetLightType(1);
        light->SetLightScale(20.f, 20.f, 1.0f);
        light->SetLightColor(40.f / 255.f, 120.f / 255.f, 10.f / 255.f, 1.f);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGravity(100.f);
        mRigid->SetRotate(true);

        mAudio = AddComponent<AudioSource>();
        mAudio->SetClip(ResourceManager::LoadAudioClip(L"bullet_arrow_01", L"..\\Resources\\Audio\\Projectiles\\bullet_arrow_01.wav"));
        mAudio->SetLoop(false);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"SineFuncParticleCS"));

        MeshRenderer* particleLight1 = new MeshRenderer();
        particleLight1->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        particleLight1->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleLight(particleLight1);

        auto particles = std::make_shared<Particle[]>(200);

        mParticleData.distance.z = 0;
        mParticleData.randLifeTime = Vector2(0.15f, 0.3f);
        mParticleData.angle = GetComponent<Transform>()->GetRotation().z;
        mParticleData.scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mParticleData.color_min = Vector4(150.f / 255.f, 255.f / 255.f, 70.f / 255.f, 1.0f);
        mParticleData.color_max = Vector4(150.f / 255.f, 255.f / 255.f, 70.f / 255.f, 1.0f);
        mParticleData.lightScale = Vector4(7.0f, 7.0f, 1.0f, 0.0f);
        mParticleData.randVelocityMax = Vector2(3.0f, 0.0f);

        mParticle->CreateStructedBuffer(sizeof(Particle), 200, eViewType::UAV, particles.get(), true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mSubParticle = AddComponent<ParticleSystem>();
        mSubParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mSubParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mSubParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCS"));

        auto subParticles = std::make_shared<Particle[]>(50);
        mSubParticle->CreateStructedBuffer(sizeof(Particle), 50, eViewType::UAV, subParticles.get(), true, 0, 14, 0);
        mSubParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShareData.color_min = mShareData.color_max = Vector4(150.f / 255.f, 255.f / 255.f, 70.f / 255.f, 1.0f);
        mShareData.randPositionMax = Vector2(3.0f, 50.0f);
        mShareData.randPositionMin = Vector2(-3.0f, -50.0f);
        mShareData.randVelocityMax = Vector2(0.0f, 5.0f);
        mShareData.randVelocityMin = Vector2(0.0f, 0.0f);
        mShareData.randLifeTime = Vector2(0.5f, 3.0f);
        mShareData.lightScale = Vector4(5.0f, 5.0f, 1.0f, 0.0f);


        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });
    }

    MagicArrow::~MagicArrow()
    {
    }

    void MagicArrow::Initialize()
    {
        mSpeed = 350.f;
        mCastDelay = 0.05f;
        mDamage = 10.0f;
        mManaDrain = 20.0f;
        mSubParticleTime = 0.f;
        mSleepTime = 0.f;
        mbTimerOn = false;
        mIndex = 0;
        mTrailDuration = 0.05f;

        mMuzzleEffect = new MuzzleEffect();
        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"muzzle_laser_green_01");
        Animator* manimator = new Animator();
        manimator->Create(L"muzzle_laser_green_01_Play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        manimator->PlayAnimation(L"muzzle_laser_green_01_Play", false);

        mMuzzleEffect->SetAnimator(manimator, L"muzzle_laser_green_01_Play");
        mMuzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        CreateObject(mMuzzleEffect, eLayerType::Effect);

        mExplosion = new ExplosionEffect();

        std::shared_ptr<Texture> explosionTexture = ResourceManager::Find<Texture>(L"explosion_016_slime");
        Animator* animator = new Animator();
        animator->Create(L"explosion_016_slime_Play", explosionTexture, Vector2(0.0f, 1.0f), Vector2(17.0f, 17.0f), 8, Vector2::Zero, 0.021f);
        animator->PlayAnimation(L"explosion_016_slime_Play", false);

        mExplosion->SetAnimator(animator, L"explosion_016_slime_Play");

        ProjectileSpell::Initialize();
    }
        
    void MagicArrow::Update()
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

    void MagicArrow::LateUpdate()
    {
        if (mbTimerOn)
        {
            GetComponent<Light>()->LateUpdate();
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

    void MagicArrow::Render()
    {
        if (mbTimerOn)
        {
            mParticle->Render();
            mSubParticle->Render();

            return;
        }

        ProjectileSpell::Render();
    }

    void MagicArrow::Dead()
    {
        if (IsActive())
        {
            SetState(eState::Sleep);

            Vector3 pos = GetComponent<Transform>()->GetPosition() - (mDirection * 350.f * 0.005f);
            mExplosion->GetComponent<Transform>()->SetPosition(pos.x, pos.y, BACK_PIXEL_WORLD_Z);
            mExplosion->GetComponent<Transform>()->SetScale(9.0f, 9.0f, 1.0f);
            CreateObject(mExplosion, eLayerType::Effect);

            GetComponent<Light>()->TrunOff();
            mbTimerOn = true;
        }
    }

    ProjectileSpell* MagicArrow::Clone()
    {
        return ObjectPoolManager::GetObjectInPool<MagicArrow>();
    }

    void MagicArrow::InitialSetting()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();
        mPrevPos = curPos + 0.0f;
        mShareData.curPosition = mPrevPos;
        mParticleData.curPosition = mPrevPos;

        mRigid->SetStartVelocity(mSpeed, mDirection);
        mAudio->Play();
    }

    void MagicArrow::OnCollision(Element& element)
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