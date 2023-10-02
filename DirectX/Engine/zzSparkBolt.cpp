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
namespace zz
{
    using namespace std;

    SparkBolt::SparkBolt()
        : mPrevPos(Vector4::Zero)
        , mParticle(nullptr)
        , mTailParticle(nullptr)
        , mSubParticle(nullptr)
        , mExplosion(nullptr)
        , mRigid(nullptr)
        , mTime(0.0f)
        , mSubParticleTime(0.f)
        , mSleepTime(3.0f)
        , mbTimerOn(false)
        , mTailData{}
        , mShareData{}
        , mIndex(0)
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
        //AddComponent<Light>()->SetLightScale(10.f, 10.f, 1.0f);

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
        mParticle->CreateStructedBuffer(sizeof(ProjectileShared), 1, eViewType::UAV, nullptr, true, 1, 14, 1);

        mSubParticle = AddComponent<ParticleSystem>();
        mSubParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mSubParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mSubParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCS"));

        Particle subParticles[50] = {};
        mSubParticle->CreateStructedBuffer(sizeof(Particle), 50, eViewType::UAV, subParticles, true, 0, 14, 0);
        mSubParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShareData.color = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 0.25f);

        mShareData.randPositionMax = Vector2(3.0f, 50.0f);
        mShareData.randPositionMin = Vector2(-3.0f, -50.0f);
        mShareData.randVelocityMax = Vector2(5.0f, -7.0f);
        mShareData.randVelocityMin = Vector2(-5.0f, 7.0f);
        mShareData.randLifeTime = Vector2(3.0f, 0.5f);

        mTailParticle = AddComponent<ParticleSystem>();
        mTailParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mTailParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mTailParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"SineFuncParticleCS"));
        
        Particle tailParticles[100] = {};
        mTailParticle->CreateStructedBuffer(sizeof(Particle), 100, eViewType::UAV, tailParticles, true, 0, 14, 0);
        mTailParticle->CreateStructedBuffer(sizeof(SineParticleShared), 1, eViewType::UAV, nullptr, true, 5, 14, 1);

        mTailData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mTailData.color = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 0.7f);

        mTailData.randPositionMax = Vector2(2.0f, 2.0f);
        mTailData.randPositionMin = Vector2(-2.0f, -2.0f);
        mTailData.randVelocityMax = Vector2(20.0f, 20.0f);
        mTailData.randVelocityMin = Vector2(10.0f, -20.0f);
        mTailData.randLifeTime = Vector2(0.2f, 0.1f);
        mTailData.angle = GetComponent<Transform>()->GetRotation().z;
        mTailParticle->SetStructedBufferData(&mTailData, 1, 1);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });

        GameObject::Initialize();

        mExplosion = new ExplosionEffect();
        std::shared_ptr<Texture> explosionTexture = ResourceManager::Find<Texture>(L"Explosion_SparkBolt");
        Animator* animator = new Animator();
        animator->Create(L"Explosion_SparkBolt_Play", explosionTexture, Vector2(0.0f, 1.0f), Vector2(9.0f, 9.0f), 8, Vector2::Zero, 0.01f);
        animator->PlayAnimation(L"Explosion_SparkBolt_Play", false);

        mExplosion->SetAnimator(animator, L"Explosion_SparkBolt_Play");

        //ExplosionEffect* muzzle = new ExplosionEffect();
        //std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"Muzzle_SparkBolt");
        //Animator* manimator = new Animator();
        //manimator->Create(L"Muzzle_SparkBolt_Play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
        //manimator->PlayAnimation(L"Muzzle_SparkBolt_Play", false);

        //muzzle->SetAnimator(manimator, L"Muzzle_SparkBolt_Play");
        ////muzzle->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetPosition().x, GetComponent<Transform>()->GetPosition().y, BACK_PIXEL_WORLD_Z);
        ////muzzle->GetComponent<Transform>()->SetRotationZ(GetComponent<Transform>()->GetRotation().z);
        //muzzle->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
        //CreateObject(muzzle, eLayerType::Effect);
    }

    void SparkBolt::Update()
    {
        if (mbTimerOn)
        {
            mSleepTime -= (float)Time::DeltaTime();

            if (mSleepTime <= 0.f)
            {
                DeleteObject(this, eLayerType::PlayerAttack);
            }
            return;
        }

        mTime += (float)Time::DeltaTime();

        Transform* tr = GetComponent<Transform>();
        Vector3 prevPos = tr->GetPosition();
        mPrevPos = Vector4(prevPos.x, prevPos.y, prevPos.z, 0.0f);


        if (mTime >= 0.8f && IsActive())
        {
            SetState(eState::Sleep);
            mbTimerOn = true;

            Vector3 pos = GetComponent<Transform>()->GetPosition();
            mExplosion->GetComponent<Transform>()->SetPosition(pos.x, pos.y, BACK_PIXEL_WORLD_Z);
            mExplosion->GetComponent<Transform>()->SetScale(9.0f, 9.0f, 1.0f);
            CreateObject(mExplosion, eLayerType::Effect);
        }

        GameObject::Update();
    }

    void SparkBolt::LateUpdate()
    {
        if (mbTimerOn)
        {
            mTailParticle->LateUpdate();
            mParticle->LateUpdate();
            mSubParticle->LateUpdate();

            return;
        }

        mTailData.curPosition = mPrevPos - mRigid->GetVelocity() * 0.05f;

        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        ProjectileShared shareData = {};
        shareData.curPosition = Vector4((int)curPos.x, (int)curPos.y, (int)curPos.z, 0.0f);
        shareData.distance = shareData.curPosition - mPrevPos;
        shareData.distance.z = 0;
        shareData.randLifeTime = Vector2(0.05f, 0.05f);

        shareData.angle = GetComponent<Transform>()->GetRotation().z;
        mPrevPos = shareData.curPosition;

        UINT count = (UINT)max(fabs(shareData.distance.x), fabs(shareData.distance.y));
        shareData.activeCount = count;
        shareData.totalActiveCount = count;
        shareData.index = mIndex;
        shareData.color = Vector4(255.f / 255.f, 80.f / 255.f, 240.f / 255.f, 1.0f);
        //mIndex += count;
        mParticle->SetStructedBufferData(&shareData, 1, 1);

        mShareData.curPosition = Vector4(curPos.x, curPos.y, curPos.z, 0.0f);
        mShareData.activeCount = randi(2);

        mSubParticle->SetStructedBufferData(&mShareData, 1, 1);

        if (mTime > 0.05f)
        {
            mIndex += count;
            mTailData.activeCount = count;
            mTailData.index = mIndex;

            mTailParticle->SetStructedBufferData(&mTailData, 1, 1);
        }

        GameObject::LateUpdate();
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

        GameObject::Render();
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