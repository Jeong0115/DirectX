#include "zzLevitation.h"
#include "zzRigidBody.h"
#include "zzInput.h"
#include "zzGameObject.h"
#include "zzTime.h"
#include "zzParticleSystem.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"
#include "zzParticleShader.h"
#include "zzTransform.h"

#include "zzEventManager.h"

#define ENERGY_DECREASE_PER_SECOND 1.0f
#define ENERGY_INCREASE_PER_SECOND_GROUND 1.5f
#define PARTICLE_SET_ACTIVE_TIME 0.05f

namespace zz
{
    Levitation::Levitation()
        : Component(eComponentType::Levitation)
        , mMaxEnergy(0.0f)
        , mCurEnergy(0.0f)
        , mParticleTime(0.0f)
        , mbFullEnergy(true)
        , mRigid(nullptr)
        , mParticle(nullptr)
        , mShareData{}
    {
    }

    Levitation::~Levitation()
    {
        delete mParticle;
    }

    void Levitation::Initialize()
    {
        mParticle = new ParticleSystem();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleCS"));

        Particle particles[100] = {};
        mParticle->CreateStructedBuffer(sizeof(Particle), 100, eViewType::UAV, particles, true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true, 4, 14, 1);

        mShareData.scale = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        mShareData.color_min = mShareData.color_max = Vector4(1.0f, 1.0f, 1.f, 0.5f);
        mShareData.randPositionMax = Vector2(3.0f, 7.0f);
        mShareData.randPositionMin = Vector2(-3.0f, -8.0f);
        mShareData.randVelocityMax = Vector2(1.0f, -2.0f);
        mShareData.randVelocityMin = Vector2(-1.0f, -5.0f);
        mShareData.randLifeTime = Vector2(0.5f, 0.2f);

        mRigid = GetOwner()->GetComponent<RigidBody>();

        mParticle->Initialize();
    }

    void Levitation::Update()
    {
        if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::SPACE))
        {
            if (mCurEnergy > 0.0f)
            {
                mRigid->SetVelocityY(50.0f);
                mRigid->SetGround(false);
                mbFullEnergy = false;
                mCurEnergy -= ENERGY_DECREASE_PER_SECOND * (float)Time::DeltaTime();
                mParticleTime += (float)Time::DeltaTime();
            }
        }

        if (mParticleTime >= PARTICLE_SET_ACTIVE_TIME)
        {
            Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();

            mShareData.curPosition = Vector4(pos.x, pos.y - 10.f, pos.z, 0.0f);
            mShareData.activeCount = randi(10) + 10;

            mParticle->SetStructedBufferData(&mShareData, 1, 1);

            mParticleTime = 0.0f;
        }

        if (!mbFullEnergy)
        {
            if (mRigid->IsGround())
            {
                mCurEnergy += ENERGY_INCREASE_PER_SECOND_GROUND * (float)Time::DeltaTime();
            }
            else
            {
                //mCurEnergy += ENERGY_INCREASE_PER_SECOND_GROUND * 0.1f * (float)Time::DeltaTime();
            }
        }

        if (mCurEnergy >= mMaxEnergy)
        {
            mCurEnergy = mMaxEnergy;
            mbFullEnergy = true;
        }

        EvenetData data;
        data.eventType = eEvent::Enenerge_Change;
        data.energe = mCurEnergy / mMaxEnergy;

        EventManager::RegisterEvent(data);
        
        mParticle->Update();
    }

    void Levitation::LateUpdate()
    {
        mParticle->LateUpdate();
    }

    void Levitation::Render()
    {
        mParticle->Render();
    }
}