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
namespace zz
{
    using namespace std;

    SparkBolt::SparkBolt()
        : mParticle(nullptr)
        , mTailParticle(nullptr)
        , mSubParticle(nullptr)
        , mExplosion(nullptr)
        , mSubParticleTime(0.f)
        , mSleepTime(0.0f)
        , mbTimerOn(false)
        , mTailData{}
        , mShareData{}
        , mIndex(0)
        , mTrailDuration(0.05f)
    {
        mSpeed = 500.f;
        mCastDelay = 0.05f;
        mDamage = 3.0f;
        mManaDrain = 5.0f;
    }

    SparkBolt::~SparkBolt()
    {
    }

    void SparkBolt::Initialize()
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

        mSound = ResourceManager::Find<AudioClip>(L"SparkBolt_Sound");


        mRigid = AddComponent<RigidBody>();
        mRigid->SetStartVelocity(mSpeed, mDirection);
        mRigid->SetGravity(100.f);
        mRigid->SetRotate(true);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ProjectileParticleCS"));

        Particle particles[100] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 100, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mSubParticle = AddComponent<ParticleSystem>();
        mSubParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mSubParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mSubParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCS"));

        Particle subParticles[50] = {};
        mSubParticle->CreateStructedBuffer(sizeof(Particle), 50, eViewType::UAV, subParticles, true, 0, 14, 0);
        mSubParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShareData.color_min = mShareData.color_max = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 0.25f);

        mShareData.randPositionMax = Vector2(30.0f, 30.0f);
        mShareData.randPositionMin = Vector2(-30.0f, -30.0f);
        mShareData.randVelocityMax = Vector2(15.0f, -30.0f);
        mShareData.randVelocityMin = Vector2(-15.0f, 30.0f);
        mShareData.randLifeTime = Vector2(3.0f, 0.5f);

        mTailParticle = AddComponent<ParticleSystem>();
        mTailParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mTailParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mTailParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"SineFuncParticleCS"));
        
        Particle tailParticles[100] = {};
        mTailParticle->CreateStructedBuffer(sizeof(Particle), 100, eViewType::UAV, tailParticles, true, 0, 14, 0);
        mTailParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mTailData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mTailData.color_min = mTailData.color_max = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 0.7f);

        mTailData.randPositionMax = Vector2(2.0f, 2.0f);
        mTailData.randPositionMin = Vector2(-2.0f, -2.0f);
        mTailData.randVelocityMax = Vector2(20.0f, 20.0f);
        mTailData.randVelocityMin = Vector2(10.0f, -20.0f);
        mTailData.randLifeTime = Vector2(0.2f, 0.1f);
        mTailData.angle = GetComponent<Transform>()->GetRotation().z;
        mTailParticle->SetStructedBufferData(&mTailData, 1, 1);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });

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

        ProjectileSpell::Initialize();
    }

    void SparkBolt::Update()
    {
        if (mbTimerOn)
        {
            mSleepTime += (float)Time::DeltaTime();

            if (mSleepTime >= 3.f)
            {
                DeleteObject(this, eLayerType::PlayerAttack);
            }
            return;
        }


        if (mTime >= 0.8f && IsActive())
        {
            SetState(eState::Sleep);
            mbTimerOn = true;

            Vector3 pos = GetComponent<Transform>()->GetPosition();
            mExplosion->GetComponent<Transform>()->SetPosition(pos.x, pos.y, BACK_PIXEL_WORLD_Z);
            mExplosion->GetComponent<Transform>()->SetScale(9.0f, 9.0f, 1.0f);
            CreateObject(mExplosion, eLayerType::Effect);
        }

        ProjectileSpell::Update();
    }

    void SparkBolt::LateUpdate()
    {
        if (mbTimerOn)
        {
            if (mSleepTime < mTrailDuration)
            {
                mTailData.curPosition += ((mRigid->GetVelocity() * (float)Time::DeltaTime()) + 0.0f);
                Vector4 distance = mTailData.curPosition - (mPrevPos - ((mRigid->GetVelocity() * mTrailDuration) + 0.0f));

                int count = max(fabs(distance.x), fabs(distance.y));

                if (count > 0)
                {
                    mIndex += count;
                    mPrevPos += distance;

                    mTailData.activeCount = count;
                    mTailData.index = (float)mIndex;

                    mTailParticle->SetStructedBufferData(&mTailData, 1, 1);
                }
            }
            mTailParticle->LateUpdate();
            mParticle->LateUpdate();
            mSubParticle->LateUpdate();

            return;
        }

        mTailData.curPosition = mPrevPos - mRigid->GetVelocity() * mTrailDuration;

        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        ParticleShared shareData = {};
        shareData.curPosition = curPos + 0.0f;
        shareData.distance = shareData.curPosition - mPrevPos;
        shareData.distance.z = 0;
        shareData.randLifeTime = Vector2(0.05f, 0.05f);
        shareData.angle = GetComponent<Transform>()->GetRotation().z;

        UINT count = (UINT)max(fabs(shareData.distance.x), fabs(shareData.distance.y));
        shareData.activeCount = count;
        shareData.totalActiveCount = count;
        shareData.index = mIndex;
        shareData.color_min = shareData.color_max = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 1.0f);
        //mIndex += count;
        mParticle->SetStructedBufferData(&shareData, 1, 1);

        mShareData.curPosition = curPos + 0.0f;
        mShareData.activeCount = random() > 0.9f ? 1 : 0;

        mSubParticle->SetStructedBufferData(&mShareData, 1, 1);

        if (mTime > 0.05f)
        {
            mIndex += count;
            mTailData.activeCount = count;
            mTailData.index = mIndex;

            mTailParticle->SetStructedBufferData(&mTailData, 1, 1);
        }

        ProjectileSpell::LateUpdate();
    }

    void SparkBolt::Render()
    {
        if (mbTimerOn)
        {
            mParticle->Render();
            mTailParticle->Render();
            mSubParticle->Render();

            return;
        }

        ProjectileSpell::Render();
    }

    ProjectileSpell* SparkBolt::Clone()
    {
        return new SparkBolt();
    }

    void SparkBolt::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
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
        else if (element.Type == eElementType::LIQUID)
        {
            mRigid->ApplyResistance(pow((0.0001f), (float)Time::DeltaTime()));
        }
    }
}