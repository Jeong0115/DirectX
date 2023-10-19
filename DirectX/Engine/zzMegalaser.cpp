#include "zzMegalaser.h"
#include "zzAnimator.h"
#include "zzTransform.h"

#include "zzMesh.h"
#include "zzTexture.h"
#include "zzMaterial.h"
#include "zzResourceManager.h"
#include "zzParticleSystem.h"

#include "zzPixelCollider_Lite.h"
#include "zzExplosionEffect.h"
#include "zzAudioSource.h"
#include "zzMegalaserProj.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzObjectPoolManager.h"

namespace zz
{
    Megalaser::Megalaser()
        : mSharedData{}
        , mParticle(nullptr)
        , mParticleTime(0.0f)
        , mbCreate(true)
        , mOffset(Vector3(-33.f,0.0f,0.0f))
    {
        mAudio = AddComponent<AudioSource>();
        mAudio->SetClip(ResourceManager::LoadAudioClip(L"megalaser_create", L"..\\Resources\\Audio\\Projectiles\\megalaser_create.wav"));
        mAudio->SetLoop(false);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        std::shared_ptr< ParticleShader> a = ResourceManager::Find<ParticleShader>(L"ParticleImageCS");
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleImageCS"));

        auto mParticleBuffer = std::make_shared<Particle[]>(3364);

        mParticle->CreateStructedBuffer(sizeof(Particle), 3364, eViewType::UAV, mParticleBuffer.get(), true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleImageShared), 1, eViewType::UAV, nullptr, true, 7, 14, 1);
        mParticle->SetGroupCount(2, 2, 1);

        MeshRenderer* particleLight = new MeshRenderer();
        particleLight->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        particleLight->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleLight(particleLight);

        mSharedData.imageSize = Vector2(58.f, 58.f);
        mSharedData.randLifeTime = Vector2(0.5f, 2.3f);
        mSharedData.randVelocityMin = Vector2(-2.0f, -2.0f);
        mSharedData.randVelocityMax = Vector2(2.0f, 2.0f);
        mSharedData.scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mSharedData.lightScale = Vector4(9.0f, 9.0f, 1.0f, 1.0f);

    }
    Megalaser::~Megalaser()
    {
    }

    void Megalaser::Initialize()
    {
        mSpeed = 150.f;
        mCastDelay = 1.5f;
        mDamage = 125.f;
        mManaDrain = 110.f;

        mParticleTime = 0.0f;
        mbCreate = false;

        mMuzzleEffect = new MuzzleEffect();
        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"muzzle_launcher_01");
        Animator* manimator = new Animator();
        manimator->Create(L"muzzle_launcher_01_play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        manimator->PlayAnimation(L"muzzle_launcher_01_play", false);

        mMuzzleEffect->SetAnimator(manimator, L"muzzle_launcher_01_play");
        mMuzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        CreateObject(mMuzzleEffect, eLayerType::Effect);

        ProjectileSpell::Initialize();
    }
    void Megalaser::Update()
    {
        ProjectileSpell::Update();
    }
    void Megalaser::LateUpdate()
    {
        renderer::ColorRangeCB colorCB = {};

        colorCB.colorRange.x = mParticleTime;
        mParticleTime += (float)Time::DeltaTime() * 2.0f;
        colorCB.colorRange.y = mParticleTime;

        if (mParticleTime >= 1.2f && !mbCreate)
        {
            for (int i = 0; i < 5; i++)
            {
                MegalaserProj* child = new MegalaserProj(this, i);
                child->SetDirection(mDirection);
                child->SetSpeed(mSpeed * 5.f);
                child->SetDamage(mDamage / 5.f);
                Transform* childTr = child->GetComponent<Transform>();

                childTr->SetPosition(GetComponent<Transform>()->GetPosition());
                childTr->SetRotation(GetComponent<Transform>()->GetRotation());

                CreateObject(child, eLayerType::PlayerAttack);
            }
            mbCreate = true;
        }
        else if (mParticleTime >= 5.0f)
        {
            mParticle->OffParticle();
            ObjectPoolManager::ReturnObjectToPool(this);
        }
        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::ColorRange];
        cb->SetBufferData(&colorCB);
        cb->BindConstantBuffer(eShaderStage::CS);

        std::shared_ptr<Texture> particleImage = ResourceManager::Find<Texture>(L"wand_64");
        particleImage->BindShader(eShaderStage::CS, 10);
        ProjectileSpell::LateUpdate();
    }
    void Megalaser::Render()
    {
        ProjectileSpell::Render();
    }

    ProjectileSpell* Megalaser::Clone()
    {
        return ObjectPoolManager::GetObjectInPool<Megalaser>();
    }

    void Megalaser::InitialSetting()
    {
        mAudio->Play();

        float angle = GetComponent<Transform>()->GetRotation().z;
        Vector3 offsetPos;
        offsetPos.x = mOffset.x * cos(angle) - mOffset.y * sin(angle);
        offsetPos.y = mOffset.x * sin(angle) + mOffset.y * cos(angle);

        mSharedData.angle = angle;
        mSharedData.color_min = mSharedData.color_max = Vector4(0.2f, 0.6f, 0.0f, 0.1f);
        mSharedData.curPosition = (GetComponent<Transform>()->GetPosition() + offsetPos) + 0.0f;
        mParticle->SetStructedBufferData(&mSharedData, 1, 1);
    }

    void Megalaser::OnCollision(Element& element)
    {
    }
}