#include "zzBouncingBurst.h"
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
#include "zzObjectPoolManager.h"
using namespace std;
namespace zz
{
    BouncingBurst::BouncingBurst()
        : mParticle(nullptr)
    {
        mSpeed = 450.f;
        mCastDelay = -0.03f;
        mDamage = 3.0f;
        mManaDrain = 5.0f;

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"rubber_ball");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"rubber_ball_idle", texture, Vector2(0.0f, 0.0f), Vector2(4.0f, 4.0f), 2, Vector2::Zero, 0.09f);
        ani->PlayAnimation(L"rubber_ball_idle", true);

        mAudio = AddComponent<AudioSource>();
        mAudio->SetClip(ResourceManager::LoadAudioClip(L"bullet_bounce_01", L"..\\Resources\\Audio\\Projectiles\\bullet_bounce_01.wav"));
        mAudio->SetLoop(false);

        GetComponent<Transform>()->SetScale(4.f, 4.f, 1.0f);
        AddComponent<Collider>()->SetScale(6.f, 6.f, 1.0f);

        Light* light = AddComponent<Light>();

        light->SetLightType(1);
        light->SetLightScale(10.f, 10.f, 1.0f);
        light->SetLightColor(40.f / 255.f, 120.f / 255.f, 10.f / 255.f, 1.f);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGravity(100.f);
        mRigid->SetRotate(true);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ProjectileParticleCS"));

        mSharedData.distance.z = 0;
        mSharedData.randLifeTime = Vector2(0.2f, 0.4f);
        mSharedData.angle = GetComponent<Transform>()->GetRotation().z;
        mSharedData.scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mSharedData.color_min = Vector4(100.f / 255.f, 200.f / 255.f, 60.f / 255.f, 1.0f);
        mSharedData.color_max = Vector4(120.f / 255.f, 240.f / 255.f, 80.f / 255.f, 1.0f);
        //mSharedData.randPositionMax = Vector2(1.0f, 0.0f);
        //mSharedData.randPositionMin = Vector2(-1.0f, 0.0f);
        mSharedData.randVelocityMax = Vector2(0.0f, 10.0f);
        mSharedData.randVelocityMin = Vector2(0.0f, -10.0f);

        mSharedData.lightScale = Vector4(3.0f, 3.0f, 1.0f, 1.0f);

        MeshRenderer* particleLight1 = new MeshRenderer();
        particleLight1->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        particleLight1->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleLight(particleLight1);

        auto particles = std::make_shared<Particle[]>(350);

        mParticle->CreateStructedBuffer(sizeof(Particle), 350, eViewType::UAV, particles.get(), true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });
    }
    BouncingBurst::~BouncingBurst()
    {
    }
    void BouncingBurst::Initialize()
    {
        mTime = 0.f;
        mSpeed = 450.f;
        mCastDelay = -0.03f;
        mDamage = 3.0f;
        mManaDrain = 5.0f;

        mTrailElement.clear();

        mMuzzleEffect = new MuzzleEffect();
        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"muzzle_medium_05");
        Animator* manimator = new Animator();
        manimator->Create(L"muzzle_medium_05_play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        manimator->PlayAnimation(L"muzzle_medium_05_play", false);

        mMuzzleEffect->SetAnimator(manimator, L"muzzle_medium_05_play");
        mMuzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        CreateObject(mMuzzleEffect, eLayerType::Effect);
       
        mAudio->Play();
      

        ProjectileSpell::Initialize();
    }
    void BouncingBurst::Update()
    {
        if (mTime >= 4.5f)
        {
            Dead();
        }
        ProjectileSpell::Update();
    }
    void BouncingBurst::LateUpdate()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        mSharedData.curPosition = curPos + 0.0f;
        mSharedData.distance = mSharedData.curPosition - mPrevPos;

        UINT count = (UINT)max(fabs(mSharedData.distance.x), fabs(mSharedData.distance.y)) + 1;
        mSharedData.activeCount = count;
        mSharedData.totalActiveCount = count;
        mSharedData.angle = GetComponent<Transform>()->GetRotation().z;
        mParticle->SetStructedBufferData(&mSharedData, 1, 1);

        ProjectileSpell::LateUpdate();
    }
    void BouncingBurst::Render()
    {
        ProjectileSpell::Render();
    }
    void BouncingBurst::Dead()
    {
        mParticle->OffParticle();
        ObjectPoolManager::ReturnObjectToPool(this);
    }

    void BouncingBurst::InitialSetting()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();
        mPrevPos = curPos + 0.0f;
        mSharedData.curPosition = mPrevPos;

        mRigid->SetStartVelocity(mSpeed, mDirection);
    }

    ProjectileSpell* BouncingBurst::Clone()
    {
        return ObjectPoolManager::GetObjectInPool<BouncingBurst>();
    }

    void BouncingBurst::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            GetComponent<PixelCollider_Lite>()->SetPositionPrevCollision();

            Vector3 vel = mRigid->GetVelocity();
            mRigid->SetVelocity(vel * -1.f * 0.8f);
        }
    }
}