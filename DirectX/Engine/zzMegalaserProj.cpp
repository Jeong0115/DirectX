#include "zzMegalaserProj.h"
#include "zzRigidBody.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzCollider.h"
#include "zzExplosionEffect.h"
#include "zzElement.h"
#include "zzTime.h"
#include "zzParticleSystem.h"
#include "zzLight.h"

namespace zz
{
    MegalaserProj::MegalaserProj(Megalaser* parent, int index)
        : mParent(parent)
        , mIndex(index)
        , mExplosion(nullptr)
        , mSleepTime(0.0f)
        , mbTimerOn(false)
        , mSharedData{}
        , mParticle(nullptr)
    {
    }

    MegalaserProj::~MegalaserProj()
    {
    }

    void MegalaserProj::Initialize()
    {
        Transform* transform = GetComponent<Transform>();
        float angle = transform->GetRotation().z;

        Vector2 offset;
        offset.x = -1.f * (mIndex - 2) * sin(angle);
        offset.y = 1.f * (mIndex - 2) * cos(angle);

        transform->ShiftPositionXY(offset);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"laser_green_big");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"laser_green_big_idle", texture, Vector2(0.0f, 0.0f), Vector2(8.0f, 5.0f), 2, Vector2::Zero, 0.02f);
        ani->PlayAnimation(L"laser_green_big_idle", true);

        GetComponent<Transform>()->SetScale(8.f, 5.f, 1.0f);
        AddComponent<Collider>()->SetScale(1.f, 1.f, 1.0f);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetStartVelocity(mSpeed, mDirection);
        mRigid->SetGravity(0.0f);
        mRigid->SetRotate(true);

        Light* light = AddComponent<Light>();
        light->SetLightType(5);
        light->SetLightScale(16.f, 16.f, 1.0f);
        light->SetLightColor(20.f / 255.f, 120.f / 255.f, 10.f / 255.f, 0.5f);

        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_plasma_fading_green"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ProjectileParticleCS"));

        Particle particles[400] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 400, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mSharedData.distance.z = 0;
        mSharedData.randLifeTime = Vector2(0.4f, 0.4f);
        mSharedData.angle = GetComponent<Transform>()->GetRotation().z;
        mSharedData.scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mSharedData.color_min = mSharedData.color_max = Vector4(150.f / 255.f, 255.f / 255.f, 70.f / 255.f, 1.0f);
        mSharedData.lightScale = Vector4(3.0f, 3.0f, 1.0f, 1.0f);

        MeshRenderer* particleLight = new MeshRenderer();
        particleLight->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        particleLight->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleLight(particleLight);

        mExplosion = new ExplosionEffect();
        std::shared_ptr<Texture> explosionTexture = ResourceManager::Find<Texture>(L"explosion_016_plasma_green");
        Animator* animator = new Animator();
        animator->Create(L"explosion_016_plasma_green_play", explosionTexture, Vector2(0.0f, 1.0f), Vector2(17.0f, 17.0f), 9, Vector2::Zero, 0.021f);
        animator->PlayAnimation(L"explosion_016_plasma_green_play", false);

        mExplosion->SetAnimator(animator, L"explosion_016_plasma_green_play");
        ProjectileSpell::Initialize();
    }

    void MegalaserProj::Update()
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
            Dead();
        }

        ProjectileSpell::Update();
    }
    void MegalaserProj::LateUpdate()
    {
        Vector3 curPos = GetComponent<Transform>()->GetPosition();

        mSharedData.curPosition = curPos + 0.0f;
        mSharedData.distance = mSharedData.curPosition - mPrevPos;

        UINT count = (UINT)max(fabs(mSharedData.distance.x), fabs(mSharedData.distance.y)) + 1;
        mSharedData.activeCount = count;
        mSharedData.totalActiveCount = count;
        mSharedData.index = (float)mIndex;
        mIndex += count;
        mParticle->SetStructedBufferData(&mSharedData, 1, 1);

        ProjectileSpell::LateUpdate();
    }
    void MegalaserProj::Render()
    {
        ProjectileSpell::Render();
    }

    void MegalaserProj::Dead()
    {
        if (IsActive())
        {
            SetState(eState::Sleep);

            Vector3 pos = GetComponent<Transform>()->GetPosition() - (mDirection * 500.f * 0.005f);
            mExplosion->GetComponent<Transform>()->SetPosition(pos.x, pos.y, BACK_PIXEL_WORLD_Z);
            mExplosion->GetComponent<Transform>()->SetScale(17.0f, 17.0f, 1.0f);
            CreateObject(mExplosion, eLayerType::Effect);

            mbTimerOn = true;
        }
    }

    ProjectileSpell* MegalaserProj::Clone()
    {
        return nullptr;
    }

    void MegalaserProj::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            Dead();
        }
    }
}